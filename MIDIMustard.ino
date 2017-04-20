///////////////////////////////////////////////

const int NUM_DIALS = 2;
#include <Bounce.h>

const int MIDI_CHANNEL = 2;

///////////////////////////////////////////////

class MIDI_DIAL
{
  static const int NUM_SAMPLES = 20;
  
  const int     m_data_pin;
  const int     m_midi_cc;
  int           m_values[NUM_SAMPLES];
  int           m_value_index;
  int           m_current_value;
    
public:

  MIDI_DIAL( int data_pin, int midi_cc ) :
    m_data_pin( data_pin ),
    m_midi_cc( midi_cc ),
    m_values(),
    m_value_index(0),
    m_current_value(0)
  {
    for( int i = 0; i < NUM_SAMPLES; ++i )
    {
      m_values[i] = 0;
    }
  }

  void update()
  {
    int average_value = 0;
    for( int i = 0; i < NUM_SAMPLES; ++i )
    {
      average_value += m_values[i];
    }
    average_value /= NUM_SAMPLES;

    const int THRESHOLD(8);
    const int new_value = analogRead( m_data_pin );
    m_values[m_value_index] = new_value;
    ++m_value_index;
    if( m_value_index >= NUM_SAMPLES )
    {
      m_value_index = 0;
    }

    const int delta = abs( new_value - average_value );
    if( delta > THRESHOLD )
    { 
      const int average_scaled  = average_value / 8;
      const int current_scaled  = m_current_value / 8;
      if( average_scaled != current_scaled )
      {
        usbMIDI.sendControlChange( m_midi_cc, average_scaled, MIDI_CHANNEL );
  
        m_current_value = average_value;
      }
    }
  }
};

///////////////////////////////////////////////

MIDI_DIAL g_dials[NUM_DIALS] = { MIDI_DIAL( 0, 41 ), MIDI_DIAL( 1, 42 ) };  // use MIDI CC 14 and 15, marked as undefined, ensures it doesn't clash with my midi pedal

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

  // consume MIDI messages
  while( usbMIDI.read() );
  
  delay(10); // 10ms
}
