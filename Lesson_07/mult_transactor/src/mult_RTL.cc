#include "mult_RTL.hh"

void mult_RTL :: elaborate_MULT_FSM(void){
  cout<<sc_simulation_time()<<" - "<<name()<<" - MULT_FSM"<<endl;
  static sc_lv<11>   e1;
  static sc_lv<11>   e2;
  static sc_lv<64>   e;
  static sc_lv<128>   m1;
  static sc_lv<128>   m2;
  static sc_lv<128>  m;
  static sc_lv<1>    s1;
  static sc_lv<1>    s2;
  static sc_lv<1>    s;
  static sc_lv<1>   sticky_bit;
  static sc_lv<1>   r;
  static sc_lv<64>   result;
  static sc_lv<64>   number1_tmp;
  static sc_lv<64>   number2_tmp;


  if (reset.read() == 0){
    //cout<< "\t" <<sc_time_stamp()<< " - mult: reset" << endl;
    STATUS = Reset_ST;
  }
  else if (clk.read() == 1) {


    STATUS = NEXT_STATUS;

    switch(STATUS){
      //reset state
      case Reset_ST:
        //cout<< "\t" <<sc_time_stamp()<< "mult: ST_reset" << endl;
        
        result_port.write('0');
        result_isready.write('0');
        overflow.write(false);
        stop_m_mult.write(false);
        normalized.write(false);
			  
        break;
        
      //initialization state
      case ST_0:
        //cout<< "\t" <<sc_time_stamp()<< "mult: ST_0" << endl;
        
        result_port.write('0');
        result_isready.write('0');
        overflow.write(false);
        stop_m_mult.write(false);
        normalized.write(false);
        
        
        e1 = 0; //11 bit
        e2 = 0; //11 bit
        e = 0; //64 bit
        
        m1 = 0; //53 bit
        m2 = 0; //53 bit
        m = 0; //128 bit
        
        s1 = 0; //1 bit
        s2 = 0; //1 bit
        s = 0; // 1 bit
        
        result = 0; // 64 bit
        sticky_bit = 0;
        r = 0;
        number1_tmp = 0;
        number2_tmp = 0;
        break;
        
      //take the numbers
      case ST_1:
        //cout<< "\t" <<sc_time_stamp()<< "mult: ST_1" << endl;
      
        number1_tmp = number1_port.read();
        number2_tmp = number2_port.read();
        
        //cout << "n1 = " << number1_tmp.to_string() << endl;
        //cout << "n2 = " << number2_tmp.to_string() << endl;
        break;
        
      //divide numbers' parts into s e m
      case ST_2:
        //cout<< "\t" <<sc_time_stamp()<< "mult: ST_2" << endl;
      
        m1 = number1_tmp.range(51, 0);
        m2 = number2_tmp.range(51, 0);
        
        e1 = number1_tmp.range(62, 52);
        e2 = number2_tmp.range(62, 52);
        
        s1 = number1_tmp.range(63, 63);
        s2 = number2_tmp.range(63, 63);
        
        m1[52] = '1';
        m2[52] = '1';
        break;
        
      //sum the exponents
      case ST_3:
        //cout<< "\t" <<sc_time_stamp()<< "mult: ST_3" << endl;
      
        /*cout << "m1 = " << m1 << " (" << m1.to_uint() << ")"<< endl;
        cout << "m2 = " << m2 << " (" << m2.to_uint() << ")"<< endl;
        cout << "e1 = " << e1 << " (" << e1.to_uint() << ")"<< endl;
        cout << "e2 = " << e2 << " (" << e2.to_uint() << ")"<< endl;
        cout << "s1 = " << s1 << endl;
        cout << "s2 = " << s2 << endl;*/
      
        e = static_cast< sc_uint<11> >( e1 ) + static_cast< sc_uint<11> >( e2 );
        break;
        
      //subtract the bias
      case ST_4:
        //cout<< "\t" <<sc_time_stamp()<< "mult: ST_4" << endl;

        //cout << "e_sum = " << e << " (" << e.to_uint() << ")"<< endl;
        e = static_cast< sc_uint<64> >( e ) - 1023;
        break;
        
      //check exp overflow
      case ST_5:
        //cout<< "\t" <<sc_time_stamp()<< "mult: ST_5" << endl;

        //cout << "e_!bias = " << e << " (" << e.to_uint() << ")"<< endl;
        if( static_cast< sc_uint<64> > (e).range(63, 11) != 0 )
          overflow.write(true);
        break;
        
      //multiplication of significand .1: check if finished
      case ST_6:
        //cout<< "\t" <<sc_time_stamp()<< "mult: ST_6" << endl;

        if(static_cast< sc_uint<53> > ( m2 ) == 0) {
          //cout << "end multiplication of significands" << endl;
          stop_m_mult.write(true);
        } else {
          //cout << "continue multiplication of significands" << endl;
          stop_m_mult.write(false);
        }
        /*cout << "m1 = " << m1 << " (" << static_cast< sc_biguint<128> >( m1 ) << ")"<< endl; 
        cout << "m2 = " << m2 << " (" << static_cast< sc_biguint<128> >( m2 ) << ")"<< endl; 
        cout << "m = " << m << " (" << static_cast< sc_biguint<128> >( m ) << ")"<< endl; */
        break;   


      //multiplication of significand .2: multiplication
      case ST_7:
        //cout<< "\t" <<sc_time_stamp()<< "mult: ST_7" << endl;
        
        //cout << "stop_m_mult = " << stop_m_mult.read() << endl;

        if(m2[0] != '0') {
          //cout << "sum m1 to m" << endl;
          m = static_cast< sc_biguint<128> >( m ) + static_cast< sc_biguint<128> >( m1 );
        }   
        
        m1 = m1 << 1;
        m2 = m2 >> 1;
        
        break;
        
      //normalization .1
      case ST_8:
        //cout<< "\t" <<sc_time_stamp()<< "mult: ST_8" << endl;
        
        if(m.range(127, 53) != 0) {
          //cout << "not normalized yet" << endl;
          normalized.write(false);
        } else {
          //cout << "normalized!" << endl;
          normalized.write(true);
        }
        
        if(e.range(10, 0) == "11111111111") {
          //cout << "overflow" << endl;
          overflow.write(true);
        } else {
          overflow.write(false);
        }
        /*cout << "m.range(127, 53) = " << m.range(127, 53) << endl;
        cout << "m.range(52, 0) = " << m.range(52, 0) << endl;
        cout << "e = " << e << " (" << e.to_uint() << ")"<< endl;*/
        
        break;
        
      //normalization .2
      case ST_9:
        //cout<< "\t" <<sc_time_stamp()<< "mult: ST_9" << endl;
        if(normalized.read() == false) {
        
          r[0] = m[0];
          sticky_bit = sticky_bit | r;
          
          m = m >> 1;
          
          e = static_cast< sc_uint<64> >( e ) + 1;
               
          
        } else {
        
          if(r == "1" && sticky_bit == "1")
            m = static_cast< sc_biguint<128> >( m ) + 1;
          else if(sticky_bit == "0" && r == "1" && m[0] == 1)
            m = static_cast< sc_biguint<128> >( m ) + 1;
            
           
        
        }
        break;
        
        
      //calculate sign
      case ST_10:
        //cout<< "\t" <<sc_time_stamp()<< "mult: ST_10" << endl;
        e = static_cast< sc_uint<64> >( e ) - 52;
        s = s1 ^ s2;
        break;
        
      //compose result
      case ST_11:
        //cout<< "\t" <<sc_time_stamp()<< "mult: ST_11" << endl;
        
        //cout << "e = " << e << " (" << e.to_uint() << ")"<< endl;
        
        result.range(63, 63) = s;
        result.range(62, 52) = e.range(10, 0);
        result.range(51, 0) = m.range(51, 0);
        break;
        
      case ST_12:
        //cout<< "\t" <<sc_time_stamp()<< "mult: ST_12" << endl;
       
        //cout << "result = " << result << endl;
      
        //cout << "exp overflow = " << overflow.read() << endl;
        result_isready.write(1);
        result_port.write( result );
        break;
        
      
    } 
  }
}


void mult_RTL :: elaborate_MULT(void){
  cout<<sc_simulation_time()<<" - "<<name()<<" - MULT"<<endl;

  NEXT_STATUS = STATUS;

  switch(STATUS){
    case Reset_ST:
      NEXT_STATUS = ST_0;
      break;

    case ST_0:
      if (numbers_areready.read() == 1){
        NEXT_STATUS = ST_1;
      } else {
        NEXT_STATUS = ST_0;
      }
      break;

    case ST_1:
      NEXT_STATUS = ST_2;
      break;
      
    case ST_2:
      NEXT_STATUS = ST_3;
      break;
      
    case ST_3:
      NEXT_STATUS = ST_4;
      break;
      
    case ST_4:
      NEXT_STATUS = ST_5;
      break;
      
    case ST_5:
      NEXT_STATUS = ST_6;
      break;
      
    case ST_6:
      if(stop_m_mult.read() == true) {
        NEXT_STATUS = ST_8;
      } else {
        NEXT_STATUS = ST_7;
      }
      break;
      
    case ST_7:
      NEXT_STATUS = ST_6;
      break;
      
    case ST_8:
      if(normalized.read() == true) {
        NEXT_STATUS = ST_10;
      } else {
        NEXT_STATUS = ST_9;
      }
      break;
      
    case ST_9:
      if(normalized.read() == true) {
        NEXT_STATUS = ST_10;
      } else {
        NEXT_STATUS = ST_8;
      }
      break;
      
    case ST_10:
      NEXT_STATUS = ST_11;
      break;
      
    case ST_11:
      NEXT_STATUS = ST_12;
      break;
      
    case ST_12:
      NEXT_STATUS = ST_0;
      break;


  }
}
