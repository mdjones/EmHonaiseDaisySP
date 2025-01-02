# Misc. production notes

## Chords

```C++
+//https://github.com/hemmer/pichenettes-eurorack-patch/blob/7738fccd4e2f10d988d0bee883903f55ef5d2132/plaits/dsp/engine/chord_engine.h
+       const float chords[kChordNumChords][kChordNumNotes] = {
+               { 0.00f, 0.01f, 11.99f, 12.00f },  // OCT
+               { 0.00f, 7.01f,  7.00f, 12.00f },  // 5
+               { 0.00f, 5.00f,  7.00f, 12.00f },  // sus4
+               { 0.00f, 3.00f,  7.00f, 12.00f },  // m
+               { 0.00f, 3.00f,  7.00f, 10.00f },  // m7
+               { 0.00f, 3.00f, 10.00f, 14.00f },  // m9
+               { 0.00f, 3.00f, 10.00f, 17.00f },  // m11
+               { 0.00f, 2.00f,  9.00f, 16.00f },  // 69
+               { 0.00f, 4.00f, 11.00f, 14.00f },  // M9
+               { 0.00f, 4.00f,  7.00f, 11.00f },  // M7
+               { 0.00f, 4.00f,  7.00f, 12.00f },  // M
+       };
++//https://github.com/hemmer/pichenettes-eurorack-patch/blob/7738fccd4e2f10d988d0bee883903f55ef5d2132/plaits/dsp/engine/chord_engine.h
+       const float chords[kChordNumChords][kChordNumNotes] = {
+               { 0.00f, 0.01f, 11.99f, 12.00f },  // OCT
+               { 0.00f, 7.01f,  7.00f, 12.00f },  // 5
+               { 0.00f, 5.00f,  7.00f, 12.00f },  // sus4
+               { 0.00f, 3.00f,  7.00f, 12.00f },  // m
+               { 0.00f, 3.00f,  7.00f, 10.00f },  // m7
+               { 0.00f, 3.00f, 10.00f, 14.00f },  // m9
+               { 0.00f, 3.00f, 10.00f, 17.00f },  // m11
+               { 0.00f, 2.00f,  9.00f, 16.00f },  // 69
+               { 0.00f, 4.00f, 11.00f, 14.00f },  // M9
+               { 0.00f, 4.00f,  7.00f, 11.00f },  // M7
+               { 0.00f, 4.00f,  7.00f, 12.00f },  // M
+       };
+
```

Some cool chord theory on BitWig
<https://www.reddit.com/r/Bitwig/comments/1hqiey8/simple_one_note_improv_trick/?share_id=eG3FRWx2efhKyojg_pAMz&utm_content=2&utm_medium=ios_app&utm_name=iossmf&utm_source=share&utm_term=22>
