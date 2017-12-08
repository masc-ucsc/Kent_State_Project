package hfvcontroller;

/**
 *
 * @author hskinner
 */
public class WaveformState {
    public int powerLevel;
    public int dutyCycle;
    public int pulseTime;
    public int pulseSpacing;
    
    public WaveformState(int p, int d, int pt, int ps) {
        powerLevel = p;
        dutyCycle = d;
        pulseTime = pt;
        pulseSpacing = ps;
    }
    
    public WaveformState() {
        powerLevel = 0;
        dutyCycle = 0;
        pulseTime = 0;
        pulseSpacing = 0;
    }
    
    public boolean equals(WaveformState other) {
        return powerLevel == other.powerLevel && dutyCycle == other.dutyCycle &&
                pulseTime == other.pulseTime && pulseSpacing == other.pulseSpacing;
    }
}
