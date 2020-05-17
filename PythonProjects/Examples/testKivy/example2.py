# import kivy module  
import kivy  
    
# this restrict the kivy version i.e  
# below this kivy version you cannot  
# use the app or software  
kivy.require("1.9.1") 
  
# Kivy Example App for the slider widget 
from kivy.app import App 
from kivy.graphics import *  

# The GridLayout arranges children in a matrix. 
from kivy.uix.gridlayout import GridLayout 
  
# If we will not import this module 
# It will through the error 
from kivy.uix.slider import Slider 
  
# The Label widget is for rendering text. 
from kivy.uix.label import Label 

# The Image widget is used to display an image 
# this module contain all features of images 
from kivy.uix.image import Image 
from kivy.uix.floatlayout import FloatLayout
  
# Property that represents a numeric value 
# within a minimum bound and / or maximum 
# bound – within a numeric range. 
from kivy.properties  import NumericProperty 
from kivy.core.window import Window

  
# class in which we are defining the 
# sliders and its effects 
class WidgetContainer(GridLayout): 
  
    def __init__(self, **kwargs): 
          
        # super function can be used to gain access  
        # to inherited methods from a parent or sibling  
        # class that has been overwritten in a class object.  
        super(WidgetContainer, self).__init__(**kwargs) 
  
        # 4 columns in grid layout 
        self.cols = 4
          
        # declaring the slider and adding some effects to it 
        self.brightnessControl = Slider(min = 0, max = 100) 
           
        # 1st row - one label, one slider     
        self.add_widget(Label(text ='Brightness')) 
        self.add_widget(self.brightnessControl)  
  
        # 2nd row - one label for caption, 
        # one label for slider value 
        self.add_widget(Label(text ='Slider Value')) 
        self.brightnessValue = Label(text ='0') 
        self.add_widget(self.brightnessValue)

        
    
        # On the slider object Attach a callback 
        # for the attribute named value 
        self.brightnessControl.bind(value = self.on_value) 
         
    # Adding functionality behind the slider 
    # i.e when pressed increase the value 
    def on_value(self, instance, brightness): 
        self.brightnessValue.text = "% d"%brightness 
  
  
# The app class 
class MainApp(App): 
    def build(self):
        self.title = 'Gets Detection'
        # Window.clearcolor = (1, 1, 1, 0)
        Window.size = (960,640)

        layout = FloatLayout(size=(300, 300))
        wimg = Image(source ='get.jpg', size_hint=(1, 1), pos=(100, 0)) 
        layout.add_widget(wimg)

        # Add a red color
        Color(1., 0, 0)

        # Add a rectangle
        rec = Rectangle(pos=(10, 10), size=(500, 500))

        layout.add_widget(rec)

        # widgetContainer = WidgetContainer() 
        # return widgetContainer
        return layout
   
  
if __name__ == '__main__':
    # creating the object root for ButtonApp() class   
    root = MainApp() 
        
    # run function runs the whole program  
    # i.e run() method which calls the  
    # target function passed to the constructor.  
    root.run()  