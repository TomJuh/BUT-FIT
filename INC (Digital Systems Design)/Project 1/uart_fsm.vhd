-- uart_fsm.vhd: UART controller - finite state machine
-- Author(s): 
--
library ieee;
use ieee.std_logic_1164.all;

-------------------------------------------------
entity UART_FSM is
port(
   CLK         : in std_logic;
   RST         : in std_logic;
   DIN         : in std_logic;
   COUNT_MAIN  : in std_logic_vector (4 downto 0);
   COUNT_WORD  : in std_logic_vector (3 downto 0);  
   RECEIVING   : out std_logic;
   M_CNT_BOOL  : out std_logic;
   VALBOOL     : out std_logic
   );
end entity UART_FSM;

-------------------------------------------------
architecture behavioral of UART_FSM is
type STATE_TYPE is (WAIT_START_BIT, WAIT_FIRST_BIT, RECEIVE_DATA, WAIT_STOP_BIT, DATA_VALID);
signal state : STATE_TYPE := WAIT_START_BIT;
begin
	RECEIVING 	<= '1' when state = RECEIVE_DATA else '0'; 
	M_CNT_BOOL      <= '1' when state = WAIT_FIRST_BIT or state = RECEIVE_DATA else '0';
	VALBOOL		<= '1' when state = DATA_VALID else '0';
	process(CLK) begin
		if rising_edge(CLK) then
            		if RST = '1' then
                    		state <= WAIT_START_BIT;
                	else
                    		case state is -- switches between states
                        		when WAIT_START_BIT =>  if DIN = '0' then
                                                    			state <= WAIT_FIRST_BIT;
                                                		end if;
                        		when WAIT_FIRST_BIT =>  if COUNT_MAIN = "10110" then
                                                   			state <= RECEIVE_DATA;
                                                		end if;
					when RECEIVE_DATA   => 	if COUNT_WORD = "1000" then
									state <= WAIT_STOP_BIT;
								end if;
					when WAIT_STOP_BIT  =>  if DIN = '1' then
									state <= DATA_VALID;
								end if;
					when DATA_VALID     =>  state <= WAIT_START_BIT;
                        		when others => null;
                        	end case;
			end if;
 		end if;
        end process;

end behavioral;
