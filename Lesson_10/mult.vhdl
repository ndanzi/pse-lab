PACKAGE mult_pack IS
  CONSTANT SIZE : INTEGER := 64;
  CONSTANT Reset_ST : INTEGER := 0;	
  CONSTANT ST_0 : INTEGER := 1;
  CONSTANT ST_1 : INTEGER := 2;
  CONSTANT ST_2 : INTEGER := 3;
  CONSTANT ST_3 : INTEGER := 4;
  CONSTANT ST_4 : INTEGER := 5;  
  CONSTANT ST_5 : INTEGER := 6;  
  CONSTANT ST_6 : INTEGER := 7;  
  CONSTANT ST_7 : INTEGER := 8;  
  CONSTANT ST_8 : INTEGER := 9;  
  CONSTANT ST_9 : INTEGER := 10;  
  CONSTANT ST_10 : INTEGER := 11;  
  CONSTANT ST_11 : INTEGER := 12;  
  CONSTANT ST_12 : INTEGER := 13;  
    
END mult_pack;
  
LIBRARY IEEE;
USE IEEE.std_logic_1164.ALL;
USE WORK.mult_pack.ALL;
--USE IEEE.NUMERIC_BIT.ALL;
use ieee.numeric_std.all;

use std.textio.all;
--use ieee.std_logic_textio.all;
--use work.txt_util.all;
  
  
--definisco l'entità MULT
entity mult is
  port (
    clock, reset   : in  bit;
    numbers_areready: in std_logic;
    number1_port: in std_logic_vector (SIZE-1 DOWNTO 0);
    number2_port: in std_logic_vector (SIZE-1 DOWNTO 0);

    result_isready: out bit;
    result_port: out std_logic_vector (SIZE-1 DOWNTO 0)
  );
  
end mult;


--definisco l'architettura di MULT
architecture mult of mult is

	subtype status_t is integer range 0 to 13;

	--definisco i segnali
	signal STATUS: status_t;
	signal NEXT_STATUS: status_t;
	signal overflow: std_logic;
	signal normalized: std_logic;
	signal stop_m_mult: std_logic;
	
	--definisco le costanti
	CONSTANT ZERO11 : std_logic_vector (10 DOWNTO 0) := "00000000000";
  CONSTANT ONE11 : std_logic_vector (10 DOWNTO 0) :=  "11111111111";
	CONSTANT ZERO53 : std_logic_vector (52 DOWNTO 0) := "00000000000000000000000000000000000000000000000000000";
	CONSTANT ZERO64 : std_logic_vector (63 DOWNTO 0) := "0000000000000000000000000000000000000000000000000000000000000000";
	CONSTANT ZERO75 : std_logic_vector (74 DOWNTO 0) := "000000000000000000000000000000000000000000000000000000000000000000000000000";    
	CONSTANT ZERO128 : std_logic_vector (127 DOWNTO 0) := "00000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000";
   
	
	
  --inizio a descrivere il comportamento della mia architettura
 begin 
   -- quello che in systemC era MULT_FSM
process(clock, reset)

   --scrivo le variabili che mi servono
   variable e1: std_logic_vector (10 DOWNTO 0);
   variable e2: std_logic_vector (10 DOWNTO 0);
   variable e: std_logic_vector (63 DOWNTO 0);
   variable m1: std_logic_vector (127 DOWNTO 0);
   variable m2: std_logic_vector (127 DOWNTO 0);
   variable m: std_logic_vector (127 DOWNTO 0);
   variable s1: std_logic;
   variable s2: std_logic;
   variable s: std_logic;
   variable sticky_bit: std_logic;
   variable r: std_logic;
   variable result: std_logic_vector (63 DOWNTO 0);
   variable number1_tmp: std_logic_vector (63 DOWNTO 0);
   variable number2_tmp: std_logic_vector (63 DOWNTO 0);

	 --file l_file: TEXT open write_mode is "C:/Lesson_10/output.log";
	 --variable  outline  : line;

begin
	if reset='0' then 
		STATUS<=Reset_ST;
	elsif clock'event and clock='1' then 
	
		STATUS<=NEXT_STATUS;

		case status is
			when Reset_ST =>
				result_port <= ZERO64;
				result_isready <= '0';
				overflow <= '0';
				normalized <= '0';
				stop_m_mult <= '0';

			when ST_0 =>
				result_port <= ZERO64;
				result_isready <= '0';
				overflow <= '0';
				normalized <= '0';
				stop_m_mult <= '0';

				e1 := ZERO11; --11 bit
				e2 := ZERO11; --11 bit
				e := ZERO64; --64 bit
				
				m1 := ZERO128; --128 bit
				m2 := ZERO128; --128 bit
				m := ZERO128; --128 bit
				
				s1 := '0'; --1 bit
				s2 := '0'; --1 bit
				s := '0'; --1 bit
				
				result := ZERO64; --64 bit
				sticky_bit := '0';
				r := '0';
				number1_tmp := ZERO64;
				number2_tmp := ZERO64;
			
			when ST_1 =>

				number1_tmp := number1_port;
				number2_tmp := number2_port;
				
				--write(outline, number1_tmp);
				--writeline(l_file, outline);
				--write(outline, number2_tmp);
				--writeline(l_file, outline);

			when ST_2 =>

				m1(51 downto 0) := number1_tmp(51 downto 0);
				m2(51 downto 0) := number2_tmp(51 downto 0);
				
				e1(10 downto 0) := number1_tmp(62 downto 52);
				e2(10 downto 0) := number2_tmp(62 downto 52);
				
				s1 := number1_tmp(63);
				s2 := number2_tmp(63);
				
				m1(52) := '1';
				m2(52) := '1';
						
			when ST_3 =>
			
				e(10 downto 0) := std_logic_vector(unsigned(e1) + unsigned(e2));
				
			when ST_4 =>
			
				e(10 downto 0) := std_logic_vector(unsigned(e(10 downto 0)) - 1023);
				
			when ST_5 =>
				
				if e(63 downto 11) /= ZERO53 then
						overflow <= '1';
				end if;
				
			when ST_6 =>
				
				if m2(52 downto 0) = ZERO53 then
						stop_m_mult <= '1';
				else    
						stop_m_mult <= '0';
				end if;  

			when ST_7 =>
				
				if m2(0) = '1' then
						m := std_logic_vector(unsigned(m) + unsigned(m1));
				end if;
				
				m1 := std_logic_vector(unsigned(m1) sll 1);
				m2 := std_logic_vector(unsigned(m2) srl 1);
				
			when ST_8 =>
				
				if m(127 downto 53) /= ZERO75 then
						normalized <= '0';
				else    
						normalized <= '1';
				end if;  
				
				if e(10 downto 0) = ONE11 then
						overflow <= '1';
				else    
						overflow <= '0';
				end if;  
				
			when ST_9 =>
				
				if normalized = '0' then
						
						r := m(0);
						sticky_bit := sticky_bit or r;
						
						m := std_logic_vector(unsigned(m) srl 1);
						
						e := std_logic_vector(unsigned(e) + 1);
						
				else
				
						if (r = '1') and (sticky_bit = '1') then
								m := std_logic_vector(unsigned(m) + 1);
						elsif (r = '1') and (sticky_bit = '0') and (m(0) = '1') then
								m := std_logic_vector(unsigned(m) + 1);
						end if;
						
				end if;
				
			when ST_10 =>
				
				e := std_logic_vector(unsigned(e) - 52);
				
				s := s1 xor s2;
				
			when ST_11 =>
				
				result(63) := s;
				result(62 downto 52) := e(10 downto 0);
				result(51 downto 0) := m(51 downto 0);
				
			when ST_12 =>
				
				result_port <= result;
				result_isready <= '1';
				
				--write(outline, result);
				--writeline(l_file, outline);
										
			when others =>
				result_port <= ZERO64;
				result_isready <= '0';
				overflow <= '0';
				normalized <= '0';
				stop_m_mult <= '0';
	
		end case;
	end if;
end process;
    
    -- quello che in systemC era MULT
  process(STATUS, numbers_areready, number1_port, number2_port, normalized, stop_m_mult)
  begin
		case STATUS is

		  when Reset_ST => 
				NEXT_STATUS<=ST_0;

		  when ST_0 => 
				if numbers_areready = '1' then
					NEXT_STATUS<=ST_1;
				else 
					NEXT_STATUS<=ST_0;
				end if;

		  when ST_1   =>
				NEXT_STATUS<=ST_2;
			
		  when ST_2   =>
				NEXT_STATUS<=ST_3;
            
		  when ST_3   =>
				NEXT_STATUS<=ST_4;
            
		  when ST_4   =>
				NEXT_STATUS<=ST_5;
            
      when ST_5   =>
				NEXT_STATUS<=ST_6;
            
      when ST_6   =>
				if stop_m_mult = '1' then
					NEXT_STATUS<=ST_8;
				else 
					NEXT_STATUS<=ST_7;
				end if;  

      when ST_7   =>
				NEXT_STATUS<=ST_6;

      when ST_8   =>
				if normalized = '1' then
					NEXT_STATUS<=ST_10;
				else 
					NEXT_STATUS<=ST_9;
				end if;  

      when ST_9   =>
				if normalized = '1' then
					NEXT_STATUS<=ST_10;
				else 
					NEXT_STATUS<=ST_8;
				end if;  

			when ST_10   =>
				NEXT_STATUS<=ST_11;
				
			when ST_11   =>
				NEXT_STATUS<=ST_12;
				
			when ST_12   =>
				NEXT_STATUS<=ST_0;

		  when others =>
				NEXT_STATUS<=STATUS;

		end case;
 	end process;

end mult; 
