# EuroMPE

There are my notes-in-progress on a Eurorack MPE implementation (MIDI to control voltage). There are currently two parts:

- [Polyphonic](./Polyphonic/Design%20notes.md), 4 to 8 voices (historical)
- [Duophonic](./Duophonic/readme.md), 2 voices

The **polyphonic** one was the first thing I explored, but it was eventually abandoned for two reasons. Firstly an eight-voice polyphonic system would be very expensive to build, and the quality I wanted. But more importantly I realised the sheer impracticality of a four or eight voice polyphonic system in Eurorack. Yes, there are quad modules aplenty. But they all have to be set identically which also means, unless you are an octopus, that they all have to be wiggled in unison and that is just nt feasible. And freakishly expensive. So this was shelved.

Somewhat later, after I had got myself a duophonic synth (ARP Odyssey) I came to realise that **duophonic** is a separate thing from fully polyphonic and from monophonic. It makes sense to use two hands and multiple fingers, because as keys lift other notes sound and this is an entirely different playing style. And I figured that MIDI Polyphonic Expression (MPE), which had always interested me, would work well with that. So the [duophonic](./Duophonic) part is where all current development is happening.
