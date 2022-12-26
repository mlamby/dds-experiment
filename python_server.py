import time
import random

from cyclonedds.core import Qos, Policy
from cyclonedds.domain import DomainParticipant
from cyclonedds.pub import Publisher, DataWriter
from cyclonedds.topic import Topic
from cyclonedds.util import duration

from pyidl.vehicles import Vehicle


# This is the publisher in the Vehicle Demo. It publishes a randomly moving
# vehicle updated every 0.1-1.0 seconds randomly. The 'Vehicle' class was
# generated from the vehicle.idl file with `idlc -l py vehicle.idl`


qos = Qos(
    Policy.Reliability.Reliable(duration(microseconds=60)),
    Policy.Deadline(duration(microseconds=10)),
    Policy.Durability.TransientLocal,
    Policy.History.KeepLast(10)
)

domain_participant = DomainParticipant(0)
topic = Topic(domain_participant, 'Vehicle', Vehicle, qos=qos)
publisher = Publisher(domain_participant)
writer = DataWriter(publisher, topic)


vehicle = Vehicle(name="Dallara IL-15", x=200, y=200)


while True:
    vehicle.x += random.choice([-1, 0, 1])
    vehicle.y += random.choice([-1, 0, 1])
    writer.write(vehicle)
    print(">> Wrote vehicle")
    time.sleep(random.random() * 0.9 + 0.1)