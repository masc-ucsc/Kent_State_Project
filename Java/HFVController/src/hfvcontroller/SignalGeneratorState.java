/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */
package hfvcontroller;

/**
 *
 * @author hskinner
 */
public class SignalGeneratorState {
    public WaveformState wave;
    public boolean on;
    
    public SignalGeneratorState(String status) {
        String[] split = status.split(";");
        wave = new WaveformState();
        
        if (split.length == 5) {
            on = split[0].equals("onn");
            wave.powerLevel = Integer.parseInt(split[1]);
            wave.dutyCycle = Integer.parseInt(split[2]);
            wave.pulseTime = Integer.parseInt(split[3]);
            wave.pulseSpacing = Integer.parseInt(split[4]);
        } else {
            on = false;
            wave.powerLevel = 0;
            wave.dutyCycle = 0;
            wave.pulseTime = 0;
            wave.pulseSpacing = 0;
        }
    }
    
    public SignalGeneratorState(WaveformState w, boolean o) {
        wave = w;
        on = o;
    }
}
