-- cpu.vhd: Simple 8-bit CPU (BrainLove interpreter)
-- Copyright (C) 2021 Brno University of Technology,
--                    Faculty of Information Technology
-- Author(s): xjuhas04
--

library ieee;
use ieee.std_logic_1164.all;
use ieee.std_logic_arith.all;
use ieee.std_logic_unsigned.all;

-- ----------------------------------------------------------------------------
--                        Entity declaration
-- ----------------------------------------------------------------------------
entity cpu is
 port (
   CLK   : in std_logic;  -- hodinovy signal
   RESET : in std_logic;  -- asynchronni reset procesoru
   EN    : in std_logic;  -- povoleni cinnosti procesoru
 
   -- synchronni pamet ROM
   CODE_ADDR : out std_logic_vector(11 downto 0); -- adresa do pameti
   CODE_DATA : in std_logic_vector(7 downto 0);   -- CODE_DATA <- rom[CODE_ADDR] pokud CODE_EN='1'
   CODE_EN   : out std_logic;                     -- povoleni cinnosti
   
   -- synchronni pamet RAM
   DATA_ADDR  : out std_logic_vector(9 downto 0); -- adresa do pameti
   DATA_WDATA : out std_logic_vector(7 downto 0); -- ram[DATA_ADDR] <- DATA_WDATA pokud DATA_EN='1'
   DATA_RDATA : in std_logic_vector(7 downto 0);  -- DATA_RDATA <- ram[DATA_ADDR] pokud DATA_EN='1'
   DATA_WREN  : out std_logic;                    -- cteni z pameti (DATA_WREN='0') / zapis do pameti (DATA_WREN='1')
   DATA_EN    : out std_logic;                    -- povoleni cinnosti
   
   -- vstupni port
   IN_DATA   : in std_logic_vector(7 downto 0);   -- IN_DATA obsahuje stisknuty znak klavesnice pokud IN_VLD='1' a IN_REQ='1'
   IN_VLD    : in std_logic;                      -- data platna pokud IN_VLD='1'
   IN_REQ    : out std_logic;                     -- pozadavek na vstup dat z klavesnice
   
   -- vystupni port
   OUT_DATA : out  std_logic_vector(7 downto 0);  -- zapisovana data
   OUT_BUSY : in std_logic;                       -- pokud OUT_BUSY='1', LCD je zaneprazdnen, nelze zapisovat,  OUT_WREN musi byt '0'
   OUT_WREN : out std_logic                       -- LCD <- OUT_DATA pokud OUT_WE='1' a OUT_BUSY='0'
 );
end cpu;


-- ----------------------------------------------------------------------------
--                      Architecture declaration
-- ----------------------------------------------------------------------------
architecture behavioral of cpu is
	 --PC
	signal pc_reg : std_logic_vector (11 downto 0);
	signal pc_inc : std_logic;
	signal pc_dec : std_logic;
 	--PTR
	signal ptr_reg : std_logic_vector (9 downto 0);
	signal ptr_inc : std_logic;
	signal ptr_dec : std_logic; 
 	--CNT
	signal cnt_reg : std_logic_vector (11 downto 0);
	signal cnt_inc : std_logic; 
	signal cnt_dec: std_logic; 
	--MX
	signal mx_select : std_logic_vector (1 downto 0);

	--States
	type states is (
		sBegin,
		sFetch,
		sDecode,
		sPcInc1,
		sPcInc2,
		sPcDec1,
		sPcDec2,
		sPtrInc,
		sPtrDec,
		sWhileStart1,
		sWhileStart2,
		sWhileStart3,
		sWhileEnd1,
		sWhileEnd2,
		sWhileEnd3,
		sWhileEnd4,
		sWhileEnd5,
		sPrint1,
        	sPrint2,
		sRead1,
		sRead2,
		sBreak1,
		sBreak2,
		sBreak3,
		sDelay,
		sNull);
	signal state       : states := sBegin;
	signal switchState : states := sFetch;
	signal nextState   : states;

	begin
	--	PC
	--	Changes cell value	
	pc: process (CLK, RESET, pc_inc, pc_dec) is 
	begin 
		if RESET  = '1' then
			pc_reg <= (others => '0');
		elsif (CLK'event) and (CLK='1') then
			if pc_inc = '1' then
				pc_reg <= pc_reg + 1;
			elsif pc_dec = '1' then 
				pc_reg <= pc_reg - 1;
			end if;
		end if;
	end process;
	--	PTR
	--	Changes pointer value
	ptr: process (CLK, RESET, ptr_inc, ptr_dec) is 
	begin 
		if RESET  = '1' then
			ptr_reg <= (others => '0');
		elsif (CLK'event) and (CLK='1') then
			if ptr_inc = '1' then
				ptr_reg <= ptr_reg + 1;
			elsif ptr_dec = '1' then 
				ptr_reg <= ptr_reg - 1;
			end if;
		end if;
	end process;
	--	CNT
	-- 	For loop 
	cnt: process (CLK, RESET, cnt_inc, cnt_dec) is
	begin
		if RESET = '1' then
			cnt_reg <= (others => '0');
		elsif (CLK'event) and (CLK='1') then
			if cnt_inc = '1' then
				cnt_reg <= cnt_reg + 1;
			elsif cnt_dec = '1' then
				cnt_reg <= cnt_reg - 1;
			end if;
		end if;
	end process;
	--	MUX  
	--	chooses value to be written
	 DATA_WDATA <= (others => '0') when mx_select = "11"
		 else DATA_RDATA - 1 when mx_select = "10"
	         else DATA_RDATA + 1 when mx_select = "01"
		 else IN_DATA when mx_select = "00";

	--	FSM 
	--	changes states on clock event
	change_state: process (CLK, RESET,EN)
	begin
		if RESET = '1' then
			state <= sBegin;
		elsif (CLK'event) and (CLK='1') then
			if EN = '1' then
				state <= nextState;
			end if;
		end if;
	end process;

	DATA_ADDR <= ptr_reg;
	CODE_ADDR <= pc_reg;
	OUT_DATA <= DATA_RDATA;

	fsm: process (state, OUT_BUSY, IN_VLD, CODE_DATA, DATA_RDATA, cnt_reg) is
	begin
	--	initialize everything to 0
		mx_select <= "00";
		pc_inc    <= '0';
		pc_dec    <= '0';
		ptr_inc   <= '0';
		ptr_dec   <= '0';
		cnt_inc   <= '0';
		cnt_dec   <= '0';
		CODE_EN   <= '0';
		DATA_EN   <= '0';
		DATA_WREN <= '0';
		IN_REQ    <= '0';
		OUT_WREN  <= '0';


		case state is
			when sBegin=>
				nextState <= sFetch;
			when sFetch=>
				CODE_EN <= '1';
				nextState  <= sDecode;
			when sDecode=>
				case CODE_DATA is 
					when x"3E" =>	nextState <= sPtrInc; 	   -- '>'
					when x"3C" =>   nextState <= sPtrDec; 	   -- '<'
					when x"2B" =>   nextState <= sPcInc1; 	   -- '+'
					when x"2D" =>   nextState <= sPcDec1; 	   -- '-'
					when x"5B" =>	nextState <= sWhileStart1; -- '['
					when x"5D" =>	nextState <= sWhileEnd1;   -- ']'
					when x"2E" =>	nextState <= sPrint1;      -- '.'
					when x"2C" =>	nextState <= sRead1;       -- ','	
					when x"7E" =>	nextState <= sBreak1;      -- '~'
					when x"00" =>	nextState <= sNull;        -- 'null'	
					when others => --Comments or others
						pc_inc <= '1';			 --PC <- PC + 1
						nextState <= sFetch;
				end case;
			--increment pointer 
			when sPtrInc=>   
				pc_inc  <= '1';					 --PTR <- PTR + 1,  
				ptr_inc <= '1'; 				 --PC <- PC + 1
				nextState  <= sFetch;
			--decrement pointer 
			when sPtrDec=>   
				ptr_dec <= '1'; 				 --PTR <- PTR - 1 
				pc_inc <= '1'; 					 --PC <- PC + 1
				nextState <= sFetch;
			--increment cell
			when sPcInc1 =>  					 --DATA_RDATA <- ram[PTR]
				DATA_EN <= '1';
				DATA_WREN <= '0';
				nextState <= sPcInc2;
			when sPcInc2 =>  					 --PC <- PC+  1
				nextState  <= sFetch;
				DATA_EN <= '1';
				DATA_WREN <= '1';
				pc_inc  <= '1';
				mx_select <= "01";
			--decrement cell
			when sPcDec1 =>  					 --DATA_RDATA <- ram[PTR]
				DATA_EN <= '1';
				DATA_WREN <= '0';
				nextState  <= sPcDec2;	
 			when sPcDec2 =>  					 --PC <- PC - 1
				mx_select <= "10";				 --ram[PTR] <- DATA_RDATA - 1
				DATA_EN <='1';
				DATA_WREN <= '1';
				pc_inc <='1';
				nextState <= sFetch;
			--start loop	
			when sWhileStart1 =>
				pc_inc <= '1';					 --PC <- PC + 1
				DATA_EN <= '1';					 --ram[PTR] <- DATA_RDATA - 1
				DATA_WREN <= '0';
				nextState <= sWhileStart2;
		    	when sWhileStart2 =>
				if (DATA_RDATA =(DATA_RDATA'range => '0')) then  --if (ram[ptr] == 0)
			    		cnt_inc <= '1';				 --cnt <- 1
			    		nextState <= sWhileStart3;
				else 
			    		nextState <= sFetch;
				end if;
		    	when sWhileStart3 =>
			    if (cnt_reg /= (cnt_reg'range => '0')) then 	 --while (cnt!=0)
				    CODE_EN <= '1';				 --c <-rom[ptr]
				    if (CODE_DATA = X"5B") then			 --if (c== '[')
					cnt_inc<= '1';				 --cnt <- cnt + 1
				    elsif (CODE_DATA = X"5D") then		 --if (c==']')
					cnt_dec <= '1';				 --cnt <- cnt - 1
				    end if;
				    pc_inc <= '1';				 --pc <- pc + 1
				    nextState <= sWhileStart3;
				else
				    nextState <= sFetch;
				end if;
			--end loop	
			when sWhileEnd1=> 
				DATA_EN <= '1'; 				 --DATA_RDATA <- ram[PTR]
				DATA_WREN <= '0';
				nextState <= sWhileEnd2;

			when sWhileEnd2=> 
				if (DATA_RDATA = (DATA_RDATA'range => '0')) then --if(ram[PTR]==0)
					pc_inc <= '1'; 				 --PC <- PC +1
					nextState <= sFetch;
				else 
					cnt_inc <= '1';				 --CNT <- 1
					pc_dec <= '1';				 --PC <- PC - 1
					switchState <= sWhileEnd4;
					nextState <= sDelay;			 --c <- rom[pc]

				end if;

			

			when sWhileEnd4=>
				if (cnt_reg = (cnt_reg'range => '0')) then
					nextState <= sFetch;
				else 						 --while(cnt!=0)
					if (CODE_DATA = x"5D") then		 --if (c=='[')
						cnt_inc <= '1';			 --CNT <- CNT + 1
					elsif (CODE_DATA = x"5B") then		 --if (c==']')
						cnt_dec <= '1';			 --CNT <- CNT - 1
					end if;
					nextState <= sWhileEnd5;
				end if;

			when sWhileEnd5=>
				if (cnt_reg = (cnt_reg'range => '0')) then	--if(CNT==0)
					pc_inc <= '1';				--pc <- pc + 1
				else 						--else
					pc_dec <= '1';				--pc <- pc -1
				end if;
				switchState <= sWhileEnd4;
				nextState <= sDelay;				--c <- rom [pc]
			--print cell
			when sPrint1=>
				DATA_EN <= '1';					--DATA_RDATA <- ram[PTR]
				DATA_WREN <= '0';
				nextState <= sPrint2;
			when sPrint2=>
				if (OUT_BUSY = '1') then			--while(OUT_BUSY)
					nextState  <= sPrint2;
				else
					OUT_WREN  <= '1';			--OUT_DATA <- ram[PTR]
					pc_inc  <= '1';				--PC <- PC + 1
					nextState <= sFetch;
				end if;
			--read value 
			when sRead1 =>
				IN_REQ <= '1';					--IN_REQ <- 1
				mx_select <= "00";				--DATA_WDATA <= IN_DATA
				nextState    <= sRead2;
			when sRead2=>
				if IN_VLD = '0' then				--while(!N_VLD)
					nextState <= sRead2;
				else 	
					DATA_EN <= '1';				--ram[PTR] <- DATA_WDATA 
					DATA_WREN <= '1';
					pc_inc  <= '1';				--PC <- PC +1
					nextState  <= sFetch;
				end if;						
			--break loop
			when sBreak1=>
				cnt_inc <= '1'; 				--CNT <- 1
				pc_inc <= '1';					--PC <- PC + 1
				switchState <= sBreak2;
				nextState <= sDelay;				--c <- romm[PC]
			when sBreak2=>
				if cnt_reg = (cnt_reg'range => '0') then	
					nextState <= sFetch;
				else 						--while (CNT!=0)
					if CODE_DATA = X"5B" then		--if (c=='[')
						cnt_inc <= '1';			--CNT <- CNT + 1
					elsif CODE_DATA = X"5D" then		--CNT <- CNT - 1
						cnt_dec <= '1';
					end if;
					pc_inc <= '1';				--PC <- PC + 1
					switchState <= sBreak2;
					nextState <= sDelay;			--c <- rom [PC]
				end if;
			when sNull=>
										--PC <-	PC
				
			when sDelay =>
				CODE_EN <= '1'; 				 --c <- rom[PC]
				nextState <= switchState;
			when others =>
				null;
			end case;

		end process;

	
end behavioral;
 
