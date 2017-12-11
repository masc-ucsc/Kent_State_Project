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
public class CameraBoxControlsState {
    public boolean up, down, left, right;
    public boolean widthUp, widthDown, heightUp, heightDown;
    
    public CameraBoxControlsState() {
        up = false;
        down = false;
        left = false;
        right = false;
        widthUp = false;
        widthDown = false;
        heightUp = false;
        heightDown = false;
    }
}
