from cyclonedds.core import Listener, Qos, Policy
from cyclonedds.domain import DomainParticipant
from cyclonedds.topic import Topic
from cyclonedds.sub import Subscriber, DataReader
from cyclonedds.util import duration

from pyidl.vehicles import Vehicle


class MyListener(Listener):
    def on_liveliness_changed(self, reader, status):
        print(f">> Liveliness event {status.alive_count}")


listener = MyListener()
qos = Qos(
    Policy.Reliability.BestEffort,
    Policy.Deadline(duration(microseconds=10)),
    Policy.Durability.TransientLocal,
    Policy.History.KeepLast(10),
)

domain_participant = DomainParticipant(0)
topic = Topic(domain_participant, "Vehicle", Vehicle, qos=qos)
print(f"QOS={qos}")
print(f"Topic name='{topic.name}' typename='{topic.typename}'")
subscriber = Subscriber(domain_participant)
reader = DataReader(domain_participant, topic, listener=listener)

for sample in reader.take_iter(timeout=duration(seconds=10)):
    print(sample)
