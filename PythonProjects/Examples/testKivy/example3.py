from kivy.app import App
from kivy.uix.widget import Widget
from kivy.uix.floatlayout import FloatLayout
from kivy.lang import Builder
from kivy.animation import Animation
from kivy.properties import NumericProperty

import math 

kv = '''
<Dial>:
    circle_id: circle_id
    size: root.size
    pos: 0, 0
    canvas:
        Rotate:
            angle: self.angle
            origin: self.center
        Color:
            rgb: 1, 0, 0
        Ellipse:    
            size: min(self.size), min(self.size)
            pos: 0.5*self.size[0] - 0.5*min(self.size), 0.5*self.size[1] - 0.5*min(self.size)
    
    Label:
        text: str(root.angle)
        origin: root.center
    
    Circle:
        id: circle_id   
        size_hint: 0, 0
        size: 50, 50
        pos: 0.5*root.size[0]-25, 0.9*root.size[1]-25
        canvas:
            Color:
                rgb: 0, 1, 0
            Ellipse:    
                size: 50, 50
                pos: self.pos              
    
'''
Builder.load_string(kv)

class Circle(Widget):
    def on_touch_down(self, touch):
        if self.collide_point(*touch.pos):
            print("small circle clicked")

class Dial(Widget):
    angle = NumericProperty(0)

    def on_touch_down(self, touch):
        if not self.circle_id.collide_point(*touch.pos):
            print("big circle clicked")

        y = (touch.y - self.center[1])
        x = (touch.x - self.center[0])
        calc = math.degrees(math.atan2(y, x))
        self.prev_angle = calc if calc > 0 else 360+calc
        self.tmp = self.angle

        return super(Dial, self).on_touch_down(touch) # dispatch touch event futher

    def on_touch_move(self, touch):
        y = (touch.y - self.center[1])
        x = (touch.x - self.center[0])
        calc = math.degrees(math.atan2(y, x))
        new_angle = calc if calc > 0 else 360+calc

        self.angle = self.tmp + (new_angle-self.prev_angle)%360

    def on_touch_up(self, touch):
        Animation(angle=0).start(self)


class DialApp(App):
    def build(self):
        return Dial()

if __name__ == "__main__":
    DialApp().run()