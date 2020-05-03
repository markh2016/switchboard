#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <time.h>

// gtk libraries 
#include <gtk/gtk.h>
#include <glib.h> 
#include <glib/gprintf.h>
#include <bcm2835.h>

/* Globale variables that must be seen by all  main and functions
 * this is done so as to determine if when creating
 * the timer that its has been created succesfully 
 * Hence we create variables timerID ,  and pointers to lbl_time
 * and an int to hold seconds which we will print to the lbl_time
 * defined in our glad file  using the event timertick 
 * This fires every 1000 ms  = 1 second 
 */

gint delay = -1 ;
guint seconds = 0 ; 
guint timerId = 0; 

guint switch_count = 0 ;

GtkWidget 	*lbl_time  = NULL ;
GtkWidget 	*btnUpdate = NULL ;
GtkWidget  	*window    = NULL;
GtkBuilder 	*builder   = NULL;
GError 		*error 	   = NULL;
GtkWidget 	*spinhrs   = NULL;
GtkWidget 	*spinmins  =  NULL ;
GtkWidget 	*spinsecs  = NULL ;
GtkWidget 	*duration  = NULL ; 
GtkWidget 	*lbl_alarm = NULL ;
GtkWidget   *lbl_Time_left = NULL ;
GtkWidget   *lbStatus  = NULL ;

// images  we need 
GtkWidget   *alarmImage = NULL ; 
GtkWidget   *Rm1Status = NULL ;
GtkWidget   *Rm2Status = NULL ;
GtkWidget   *Rm3Status = NULL ;
GtkWidget   *Rm4Status = NULL ;
GtkWidget   *Rm5Status = NULL ;
GtkWidget   *Rm6Status = NULL ;

//  sw0 to sw5 

GtkWidget	*sw0 = NULL ;
GtkWidget	*sw1 = NULL ;
GtkWidget	*sw2 = NULL ;
GtkWidget	*sw3 = NULL ;
GtkWidget	*sw4 = NULL ;
GtkWidget	*sw5 = NULL ;


GtkImage    *image = NULL   ;
gboolean    running  = FALSE ; 
gboolean    alarmtrig =FALSE ; // tells us when alram triiggered 

// The states of each switch 
gboolean    states[6] ; 

// defines pins

#define PIN0 RPI_GPIO_P1_11		// gpio_0 pin 11
#define PIN1 RPI_GPIO_P1_12		// gpio_1 pin 12
#define PIN2 RPI_GPIO_P1_13		// gpio_2 this is on pin21 
#define PIN3 RPI_GPIO_P1_15		// gpio_3 pin 15
#define PIN4 RPI_GPIO_P1_16		// gpio_4 pin 16
#define PIN5 RPI_GPIO_P1_18		// gpio_5 pin 18



void debug (const char * data)
{
	g_print("%s called.\n",data);
}

void quitBCM()
{
	bcm2835_gpio_write(PIN0, LOW);
	bcm2835_gpio_write(PIN1, LOW);
	bcm2835_gpio_write(PIN2, LOW);
	bcm2835_gpio_write(PIN3, LOW);
	bcm2835_gpio_write(PIN4, LOW);
	bcm2835_gpio_write(PIN5, LOW);
	
	bcm2835_close();	
}


/*timer tick event handler */ 

gboolean timerTick(__attribute__((unused)) gpointer userData)
{   
	
    
   const gchar *alarmtext  ;
   const gchar *currenttime ; 
    
    char temp[9]; 
    struct tm *now_tm;
    time_t now; 
  
     
		// to get current time 
		time(&now) ; 
                now_tm=localtime(&now) ;
		
			// print time in minutes, 
			// hours and seconds 
			sprintf(temp,"%02d:%02d:%02d" ,
			now_tm->tm_hour, 
			now_tm->tm_min,       
			now_tm->tm_sec) ;
			
			// update label 
			gtk_label_set_text(GTK_LABEL(lbl_time), temp); 
			
			    
            // get values of alarmtext , currenttime	 
				
			currenttime = gtk_label_get_text(GTK_LABEL(lbl_time)) ;
			
				
				if(running)
				{
					// compare time strings for equality 
				   alarmtext   = gtk_label_get_text(GTK_LABEL(lbl_alarm)) ;	
				   
				    
				    //  compare 
				    if (g_strcmp0(alarmtext, currenttime) == 0){
					g_print("Alarm activated  alarm is %s  Time is %s\n" , alarmtext ,currenttime ) ;
					gtk_image_set_from_file(GTK_IMAGE(alarmImage), "icons/Blue.ico"); 
					alarmtrig = TRUE ;
					gtk_label_set_text(GTK_LABEL(lbStatus), "Alarm Off" );  
					running = FALSE ; 
				    }
				}
				
				if (alarmtrig)
				{
					switch(switch_count)
					{
					case 0 :
						gtk_switch_set_active(GTK_SWITCH(sw0),TRUE);
							switch_count+=1 ;
						break ;
					case 1: 
							gtk_switch_set_active(GTK_SWITCH(sw1),TRUE);
								switch_count+=1 ;
						break ;
							
				    case 2: 
						gtk_switch_set_active(GTK_SWITCH(sw2),TRUE);
								switch_count+=1 ;
						break ;
					
					default :
					    switch_count=0 ;
					    alarmtrig = FALSE ;
					    g_print("switch count  = %d \n" ,switch_count) ;
					
					}// end switch case 
				}
				
	return TRUE;


} 

void on_destroy (__attribute__((unused)) GtkWidget *widget,__attribute__((unused)) gpointer data )
{
    debug(__FUNCTION__);
    quitBCM() ;
    bcm2835_close();
    gtk_main_quit ();
}

void on_BtnAlarm_Off_clicked (__attribute__((unused)) GtkWidget *widget,__attribute__((unused)) gpointer data )
{
	 char status[10] = "Alarm Off" ;
	 
	 gtk_image_set_from_file(GTK_IMAGE(alarmImage), "icons/off.ico"); 
	 
	 gtk_label_set_text(GTK_LABEL(lbStatus), status);  
	 running = FALSE ;
}

void on_btnAlarm_on_clicked(__attribute__((unused)) GtkWidget *widget,__attribute__((unused)) gpointer data )
{   
	// grab the image  
	 gtk_image_set_from_file(GTK_IMAGE(alarmImage), "icons/On.ico"); 
	 char status[10] = "Alarm  On" ;
	 gtk_label_set_text(GTK_LABEL(lbStatus), status);  
	 running = TRUE ;
}
void on_btnupdate_clicked (__attribute__((unused))GtkButton *btnupdate,__attribute__((unused))gpointer user_data)
{
    char  temp[9] ; 
    gdouble hrs = 0.0 ;
    gdouble mins = 0.0 ;
    gdouble secs =0.0 ;
    gdouble f_delay = 0.0 ; 
    
    
	
	hrs 	= gtk_spin_button_get_value(GTK_SPIN_BUTTON(spinhrs)) ;
	mins 	= gtk_spin_button_get_value(GTK_SPIN_BUTTON(spinmins)) ;
	secs 	= gtk_spin_button_get_value(GTK_SPIN_BUTTON(spinsecs)) ;
	f_delay  = gtk_spin_button_get_value(GTK_SPIN_BUTTON(duration));
	
	g_sprintf(temp,"%02d:%02d:%02d" , (int)hrs,(int)mins,(int)secs);
	
	gtk_label_set_text(GTK_LABEL(lbl_alarm), temp);  
	
	delay = (guint) f_delay ; 
	g_sprintf(temp,"%03d" ,(guint) f_delay) ; 
	gtk_label_set_text(GTK_LABEL(lbl_Time_left), temp);
}


	gboolean  on_id_sw5_state_set (__attribute__((unused))GtkSwitch *id_sw5,__attribute__((unused)) gpointer user_data)
	{
		
		
	    states[5] = gtk_switch_get_active(GTK_SWITCH(id_sw5)) ;
	    if(states[5])
	    {
	        gtk_image_set_from_file(GTK_IMAGE(Rm6Status), "icons/On.ico"); 
		bcm2835_gpio_write(PIN5, HIGH);
	    }
		else 
			{
			   gtk_image_set_from_file(GTK_IMAGE(Rm6Status), "icons/off.ico");
			   bcm2835_gpio_write(PIN5, LOW); 
		         }
				 
				 
	    //g_print("State 5 = %d \n" , states[5]) ;
	    	    
		return TRUE ; 
	} 

	gboolean on_id_sw4_state_set (__attribute__((unused))GtkSwitch *id_sw4, __attribute__((unused))gpointer user_data)
	{
		//debug(__FUNCTION__);
		
	    states[4] = gtk_switch_get_active(GTK_SWITCH(id_sw4)) ;
	        
	       if(states[4])
	       {
	        gtk_image_set_from_file(GTK_IMAGE(Rm5Status), "icons/On.ico"); 
		bcm2835_gpio_write(PIN4, HIGH);
	       } 
	          else 
			{
			    gtk_image_set_from_file(GTK_IMAGE(Rm5Status), "icons/off.ico");
			    bcm2835_gpio_write(PIN4, LOW); 
			}	 
	        
		// g_print("State 4 = %d \n" , states[4]) ;
		return TRUE ; 
	}

	gboolean on_id_sw3_state_set (__attribute__((unused))GtkSwitch *id_sw3, __attribute__((unused))gpointer user_data)
	{
		//debug(__FUNCTION__);
		
	    states[3] = gtk_switch_get_active(GTK_SWITCH(id_sw3)) ;
		
		if(states[3])
		{
			gtk_image_set_from_file(GTK_IMAGE(Rm4Status), "icons/On.ico"); 
			bcm2835_gpio_write(PIN3, HIGH);
		}
		   else {
				 gtk_image_set_from_file(GTK_IMAGE(Rm4Status), "icons/off.ico"); 
				 bcm2835_gpio_write(PIN3, LOW);
			}
			
		// g_print("State 3 = %d \n" , states[3]) ;
		return TRUE ; 
	}

	gboolean on_id_sw2_state_set (__attribute__((unused))GtkSwitch *id_sw2, __attribute__((unused))gpointer user_data)
	{
		//debug(__FUNCTION__);
		
	    states[2] = gtk_switch_get_active(GTK_SWITCH(id_sw2)) ;
	    
	     if(states[2])
	     {
	        gtk_image_set_from_file(GTK_IMAGE(Rm3Status), "icons/On.ico"); 
		bcm2835_gpio_write(PIN2, HIGH);
		
	      }
	           else {
				 gtk_image_set_from_file(GTK_IMAGE(Rm3Status), "icons/off.ico"); 
				 bcm2835_gpio_write(PIN2, LOW);
			        // g_print("State 2 = %d \n" , states[2]) ;
			}
	    
	   
		return TRUE ; 
	}
 
	gboolean on_id_sw1_state_set (__attribute__((unused))GtkSwitch *id_sw1, __attribute__((unused))gpointer user_data)
	{  
		//debug(__FUNCTION__);
		
		states[1] = gtk_switch_get_active(GTK_SWITCH(id_sw1)) ;
		
	if(states[1]){
	        gtk_image_set_from_file(GTK_IMAGE(Rm2Status), "icons/On.ico"); 
	        bcm2835_gpio_write(PIN1, HIGH);
	       } 
	       else 
		   { 
		     gtk_image_set_from_file(GTK_IMAGE(Rm2Status), "icons/off.ico"); 
		    bcm2835_gpio_write(PIN1, LOW);
		   }
				 
		
		//g_print("State 1 = %d \n" , states[1]) ;
	 
		return TRUE ;  
	}
  
	gboolean on_id_sw0_state_set (__attribute__((unused))GtkSwitch *id_sw0,__attribute__((unused)) gpointer user_data)
	{   
		//debug(__FUNCTION__);
		
		states[0] = gtk_switch_get_active(GTK_SWITCH(id_sw0)) ;
		
		if(states[0])
		{
	        gtk_image_set_from_file(GTK_IMAGE(Rm1Status), "icons/On.ico"); 
		bcm2835_gpio_write(PIN0, HIGH);
	        } 
		else
		{
		  bcm2835_gpio_write(PIN0, LOW);
		  gtk_image_set_from_file(GTK_IMAGE(Rm1Status), "icons/off.ico"); 
		  
	         }
				 	
		
		
		
		//g_print("State 0 = %d \n" , states[0]) ;
		
		return TRUE ; 
	}
  
  

 
 int main(int argc,char **argv) 
 {
   
    // set up all pins on  the RPI  and intialise port registers 
    if (!bcm2835_init())
    {
      return 1;
    }
    else 
    {
     // Set the pin to be an output
		bcm2835_gpio_fsel(PIN0, BCM2835_GPIO_FSEL_OUTP); 	// gpio_0
		bcm2835_gpio_fsel(PIN1, BCM2835_GPIO_FSEL_OUTP);	// gpio_1
		bcm2835_gpio_fsel(PIN2, BCM2835_GPIO_FSEL_OUTP);	// gpio_2
		bcm2835_gpio_fsel(PIN3, BCM2835_GPIO_FSEL_OUTP);	// gpio_3
		bcm2835_gpio_fsel(PIN4, BCM2835_GPIO_FSEL_OUTP);	// gpio_4
		bcm2835_gpio_fsel(PIN5, BCM2835_GPIO_FSEL_OUTP);	// gpio_5
    }
   
   // calls gtk init and passes any paramamters to program 
   gtk_init(&argc , &argv);
   
    builder = gtk_builder_new();

    if( gtk_builder_add_from_file (builder,"ui/clock.glade" , NULL) == 0)
     {
         printf("gtk_builder_add_from_file FAILED\n");
         g_print("%s\n",error->message);
         return EXIT_FAILURE ;
       
     }
   
   // get all the widget components from the builder file 
   
   
	window  		= GTK_WIDGET(gtk_builder_get_object (builder,"main_window"));
        lbl_time 		= GTK_WIDGET(gtk_builder_get_object (builder,"lbl_time"));
        btnUpdate 		= GTK_WIDGET(gtk_builder_get_object (builder,"btnupdate"));
        spinhrs			= GTK_WIDGET(gtk_builder_get_object (builder,"spinHrs"));
        spinmins		= GTK_WIDGET(gtk_builder_get_object (builder,"spinMinutes"));
        spinsecs 		= GTK_WIDGET(gtk_builder_get_object (builder,"spinSecs"));
        duration		= GTK_WIDGET(gtk_builder_get_object (builder,"spinDuration"));
        lbl_alarm		= GTK_WIDGET(gtk_builder_get_object (builder,"lbl_alarm"));
        lbl_Time_left		= GTK_WIDGET(gtk_builder_get_object (builder,"lbl_delayOff "));
        lbStatus        	= GTK_WIDGET(gtk_builder_get_object (builder,"lbStatus"));
        
        /*  sw0 to sw5  for which rooms  you want to set via BCM header */ 
        
        sw0 			= GTK_WIDGET(gtk_builder_get_object (builder,"id_sw0"));
        sw1 			= GTK_WIDGET(gtk_builder_get_object (builder,"id_sw1"));
        sw2 			= GTK_WIDGET(gtk_builder_get_object (builder,"id_sw2"));
        sw3 			= GTK_WIDGET(gtk_builder_get_object (builder,"id_sw3"));
        sw4 			= GTK_WIDGET(gtk_builder_get_object (builder,"id_sw4"));
        sw5 			= GTK_WIDGET(gtk_builder_get_object (builder,"id_sw5"));
        
        // get all images 
        
        alarmImage		= GTK_WIDGET(gtk_builder_get_object (builder,"alarm_image"));
        Rm1Status		= GTK_WIDGET(gtk_builder_get_object (builder,"Rm1Status"));
        Rm2Status		= GTK_WIDGET(gtk_builder_get_object (builder,"Rm2Status"));
        Rm3Status		= GTK_WIDGET(gtk_builder_get_object (builder,"Rm3Status"));
        Rm4Status		= GTK_WIDGET(gtk_builder_get_object (builder,"Rm4Status"));
        Rm5Status		= GTK_WIDGET(gtk_builder_get_object (builder,"Rm5Status"));
        Rm6Status		= GTK_WIDGET(gtk_builder_get_object (builder,"Rm6Status"));
        
     // build signals table 
     gtk_builder_connect_signals(builder,NULL);
     
    
     
     g_object_unref(builder); // release recources 
     
     // create a timer and  connect to event 
     if(timerId == 0){
       timerId = g_timeout_add(1000,timerTick,NULL);
     }
     else 
	{		g_print("Already Running\n");
		
	}
		 
     gtk_window_set_position (GTK_WINDOW(window), GTK_WIN_POS_CENTER); 
     
     gtk_widget_show_all (window);
   
     gtk_main(); 
     
   
     
     return EXIT_SUCCESS ;
 }


/*       if(delay != -1 ){						
 *            g_sprintf(temp,"%03d" , delay) ; 						
 *			  gtk_label_set_text(GTK_LABEL(lbl_Time_left), temp);
 *			  delay-= 1; // decriment delay 
 *		      }
 * 
 */ 
