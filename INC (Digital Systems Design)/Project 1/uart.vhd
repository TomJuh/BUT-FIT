-- uart.vhd: UART controller - receiving part
-- Author(s): xjuhas04
--
library ieee;
use ieee.std_logic_1164.all;
use ieee.std_logic_unsigned.all;
use ieee.numeric_std.all;

-------------------------------------------------
entity UART_RX is
port(	
    	CLK: 	    in std_logic;
	RST: 	    in std_logic;
	DIN: 	    in std_logic;
	DOUT: 	    out std_logic_vector(7 downto 0);
	DOUT_VLD:   out std_logic
);
end UART_RX;  

-------------------------------------------------
architecture behavioral of UART_RX is
signal countMain   : std_logic_vector(4 downto 0);
signal countWord   : std_logic_vector(3 downto 0);    
signal receiving   : std_logic;
signal mCntBool    : std_logic;
signal valBool     : std_logic;

begin
	FSM: entity work.UART_FSM(behavioral)
        port map (
            CLK 	    => CLK,
            RST 	    => RST,
            DIN 	    => DIN,
            COUNT_MAIN 	    => countMain,
            COUNT_WORD	    => countWord,
	    RECEIVING 	    => receiving,
	    M_CNT_BOOL	    => mCntBool,
	    VALBOOL	    => valBool
        );
	DOUT_VLD <= valBool;
	process (CLK) begin
		if rising_edge(CLK) then
			if mCntBool = '1' then
				countMain <= countMain  + 1;
			else

				countMain <= "00000"; --initializes value and restarts it when not needed
				
			end if;
			if receiving = '1' then -- if in receiving state
				if countMain(4) = '1' then
					countMain <= "00000"; 	--restarts the value if it reaches 16+
					if ((to_integer(unsigned(countWord)) >= 0) and (to_integer(unsigned(countWord)) <= 7)) then -- if counterWord is between 0 and 7
						DOUT(to_integer(unsigned(countWord))) <= DIN; --pushes DIN to that array index
					end if;	
					countWord <= countWord + 1;

				end if;
			else
				countWord<= "0000"; --initializes value and then restarts it after each word
			end if;
		end if;
	end process;
end behavioral;
