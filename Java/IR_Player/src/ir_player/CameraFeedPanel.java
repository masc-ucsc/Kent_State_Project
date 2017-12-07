/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */
package ir_player;

import java.awt.Color;
import java.awt.image.BufferedImage;
import java.awt.Rectangle;
import java.awt.Graphics;


/**
 *
 * @author hskinner
 */
public class CameraFeedPanel extends javax.swing.JPanel {
    private BufferedImage image = null;
    private Rectangle focusBox = null;
    
    private static final Color RECT_COLOR = Color.RED;
    private static final int INITIAL_X = 20;
    private static final int INITIAL_Y = 20;
    private static final int INITIAL_WIDTH = 100;
    private static final int INITIAL_HEIGHT = 100;
    
    public CameraFeedPanel() {
        super();
        focusBox = new Rectangle(INITIAL_X, INITIAL_Y, INITIAL_WIDTH, INITIAL_HEIGHT);
    }
    
    public void updateImage(BufferedImage nimg) { image = nimg; }
    
    protected void paintComponent(Graphics g) {
        super.paintComponent(g);
        
        if (image != null) {
            g.drawImage(image, 0, 0, null);
        }
        
        g.setColor(RECT_COLOR);
        g.drawRect(focusBox.x, focusBox.y, focusBox.width, focusBox.height);
    }
}
