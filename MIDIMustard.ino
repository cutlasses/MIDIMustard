///////////////////////////////////////////////

const int NUM_DIALS = 2;
#include <Bounce.h>

const int MIDI_CHANNEL = 2;

///////////////////////////////////////////////

class MIDI_DIAL
{
  const int     m_data_pin;
  const int     m_midi_cc;
  int           m_current_value;
    
public:

   MIDI_DIAL( int data_pin, int midi_cc ) :
    m_data_pin( data_pin ),
    m_midi_cc( midi_cc ),
    m_current_value( 0 )
  {
  
  }

  bool update()
  {
    int new_value = analogRead( m_data_pin );
  
    if( new_value != m_current_value )
    {
      m_current_value = new_value;

      usbMIDI.sendControlChange( m_midi_cc, m_current_value, MIDI_CHANNEL );
      
      return true;
    }
  
    return false;
  }
  
  float value() const
  {
    return m_current_value / 1024.0f;
  }
};

///////////////////////////////////////////////

MIDI_DIAL g_dials[NUM_DIALS] = { MIDI_DIAL( 0, 14 ), MIDI_DIAL( 1, 15 ) };  // use MIDI CC 14 and 15, marked as undefined, ensures it doesn't clash with my midi pedal

///////////////////////////////////////////////

void setup()
{
  // put your setup code here, to run once:
}

void loop()
{
  // put your main code here, to run repeatedly:
  for( int i = 0; i < NUM_DIALS; ++i )
  {
    g_dials[i].update();
  }
}
