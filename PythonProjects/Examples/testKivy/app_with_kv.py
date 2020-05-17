'''
Application built from a  .kv file
==================================

This shows how to implicitly use a .kv file for your application. You
should see a full screen button labelled "Hello from test.kv".

After Kivy instantiates a subclass of App, it implicitly searches for a .kv
file. The file test.kv is selected because the name of the subclass of App is
TestApp, which implies that kivy should try to load "test.kv". That file
contains a root Widget.
'''

import kivy
kivy.require('1.9.0')

from kivy.app import App
from kivy.core.window import Window

from kivy.uix.floatlayout import FloatLayout
from kivy.properties import NumericProperty

class MainLayout(FloatLayout):
    angle = NumericProperty(0)

    def setAngle(self, value, object):
        print('Nuevo valor: %0.f'%value)
        # Get Axis Center
        print(object.size)
        print(object.pos)
        print(object.texture_size)
        print(object.norm_image_size)
        posx = object.pos[0] + object.size[0]*0.5 - object.norm_image_size[0]*0.33739
        posy = object.pos[1] + object.size[1]*0.5 - object.norm_image_size[1]*0.20427
        print('Posx: %0.f'%posx)
        print('Posy: %0.f'%posy)
        self.angle = value 

# The app class 
class MainApp(App):
    def build(self):
        self.title = 'Gets Detection'
        #self.icon = 'ic_logo.png'
        # Window.clearcolor = (1, 1, 1, 0)
        Window.size = (960, 640)
        Window.minimum_width = 960
        Window.minimum_height = 640
        # Window.borderless = False
        # Window.fullscreen = True
        Window.show_cursor = True
        
        
        return MainLayout()
    
if __name__ == '__main__':
    root = MainApp()
    root.run()