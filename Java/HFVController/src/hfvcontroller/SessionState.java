package hfvcontroller;

/**
 *
 * @author hskinner
 */
public class SessionState {
    public int powerLevel;
    public int dutyCycle;
    public int pulseTime;
    public int pulseSpacing;
    
    public SessionState(String status) {
        String[] split = status.split(";");
        
        if (split.length == 5) {
            powerLevel = Integer.parseInt(split[1]);
            dutyCycle = Integer.parseInt(split[2]);
            pulseTime = Integer.parseInt(split[3]);
            pulseSpacing = Integer.parseInt(split[4]);
        } else {
            powerLevel = 0;
            dutyCycle = 0;
            pulseTime = 0;
            pulseSpacing = 0;
        }
    }
    
    public SessionState(int p, int d, int pt, int ps) {
        powerLevel = p;
        dutyCycle = d;
        pulseTime = pt;
        pulseSpacing = ps;
    }
    
    public boolean equals(SessionState other) {
        return powerLevel == other.powerLevel && dutyCycle == other.dutyCycle &&
                pulseTime == other.pulseTime && pulseSpacing == other.pulseSpacing;
    }
}
