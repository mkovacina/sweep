package flightController;

import javax.swing.*;

public class Main extends JFrame
{
    protected Main( String in, String out )
    {
        super( "SWEEP Flight Controller" );
        
        getContentPane().add( new Flight_Controller( in, out ) );
        
        setSize( 400, 400 );
        setDefaultCloseOperation( JFrame.EXIT_ON_CLOSE );
        show();
    }
    
    public static void main (String args[])
    {
        if ( args.length == 2 )
        {
            new Main( args[0], args[1] );
        }
        else
        {
            System.out.println( "Usage: java flightController.Main <input_file> <output_file>" );
        }
    }
}
