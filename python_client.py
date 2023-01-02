from cyclonedds.core import Listener, WaitSet, ReadCondition, ViewState, InstanceState, SampleState
from cyclonedds.domain import DomainParticipant
from cyclonedds.topic import Topic
from cyclonedds.sub import DataReader
from cyclonedds.util import duration
import cyclonedds

import PySimpleGUI as sg
import threading
from typing import List

from pyidl.entities import EntityPosition


THREAD_EVENT = '-THREAD-'


def thread_fn(window, wait_fn):
    while True:
        wait_fn()
        window.write_event_value(THREAD_EVENT, None)


def run_gui(wait_fn, read_fn):
    layout = [      
            [sg.Graph(canvas_size=(800, 800), graph_bottom_left=(0,0), graph_top_right=(800, 800), background_color='white', key='graph')],      
            [sg.Button('Move')]      
            ]      

    window = sg.Window('Graph test', layout, finalize=True)       

    graph = window['graph']     
    circle = graph.DrawCircle((75,75), 5, fill_color='black',line_color='white')

    threading.Thread(target=thread_fn, args=(window,wait_fn), daemon=True).start() 

    while True:      
        event, values = window.read()      
        if event == sg.WIN_CLOSED:      
            break     
        elif event == 'Move':         
            graph.MoveFigure(circle, 10,10)
        elif event == THREAD_EVENT:
            samples = read_fn()
            for e in samples:
                print(e.sample_info)
                graph.RelocateFigure(circle, int(e.x), int(e.y))

    window.close()


class MyListener(Listener):
    def on_liveliness_changed(self, reader, status):
        print(f">> Liveliness event {status.alive_count}")


def setup_dds():
    listener = MyListener()
    domain_participant = DomainParticipant(0)
    topic = Topic(domain_participant, "EntityPosition", EntityPosition)
    reader = DataReader(domain_participant, topic, listener=listener)
    
    waitset = WaitSet(domain_participant)
    condition = ReadCondition(reader, ViewState.Any | InstanceState.Alive | SampleState.NotRead)
    waitset.attach(condition)

    def read_values() -> List[EntityPosition]:
        return reader.take(condition=condition)

    def wait_for_sample():
        waitset.wait(duration(seconds=10))

    return read_values, wait_for_sample


def main():
    read_fn, wait_fn = setup_dds()
    run_gui(wait_fn, read_fn)


if __name__ == "__main__":
    main()
