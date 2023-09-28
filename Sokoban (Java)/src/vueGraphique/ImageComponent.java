package vueGraphique;

import java.awt.*;

public class ImageComponent extends Component{
    private static final long serialVersionUID = 1;

    private final Image img;

    public ImageComponent (Image img) {
        this.img = img;
    }

    /*dessine une image facilement */

    @Override
    public void paint (Graphics g) {
        g.drawImage(img, 0, 0, 30, 30, null, null);
    }
}
