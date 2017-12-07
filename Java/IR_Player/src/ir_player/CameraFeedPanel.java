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
    private static final int FBOX_TICK = 3;
    
    
    public CameraFeedPanel() {
        super();
        focusBox = new Rectangle(MainFrame.INITIAL_X,
                MainFrame.INITIAL_Y, MainFrame.INITIAL_WIDTH, MainFrame.INITIAL_HEIGHT);
    }
    
    public void updateImage(BufferedImage nimg) { image = nimg; }
    
    protected void paintComponent(Graphics g) {
        super.paintComponent(g);
        
        if (image != null) {
            g.drawImage(image, 0, 0, null);
            g.setColor(RECT_COLOR);
            g.drawRect(focusBox.x, focusBox.y, focusBox.width, focusBox.height);
        }
    }
    
    public void incFBoxX() { focusBox.x += FBOX_TICK; }
    public void incFBoxY() { focusBox.y += FBOX_TICK; }
    public void incFBoxWidth() { focusBox.width += FBOX_TICK; }
    public void incFBoxHeight() { focusBox.height += FBOX_TICK; }
    
    public void decFBoxX() { focusBox.x -= FBOX_TICK; }
    public void decFBoxY() { focusBox.y -= FBOX_TICK; }
    public void decFBoxWidth() { focusBox.width -= FBOX_TICK; }
    public void decFBoxHeight() { focusBox.height -= FBOX_TICK; }
    
    public void setFBoxX(int v) { focusBox.x = v; }
    public void setFBoxY(int v) { focusBox.y = v; }
    public void setFBoxWidth(int v) { focusBox.width = v; }
    public void setFBoxHeight(int v) { focusBox.height = v; }
    
    public Rectangle getFocusBox() { return focusBox; }
}
