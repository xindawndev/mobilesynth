package org.thebends.mobilesynth;

import android.app.Activity;
import android.os.Bundle;
import org.thebends.synth.Oscillator;
import org.thebends.synth.FixedParameter;

public class MobileSynthActivity extends Activity
{
    private Oscillator osc;
    private SynthTrack t;

    /** Called when the activity is first created. */
    @Override
    public void onCreate(Bundle savedInstanceState)
    {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.main);

        osc = new Oscillator(FixedParameter.get(1000), 44100);
        t = new SynthTrack(osc);
    }
}
