// Copied from JW-Modules. See ./JW_LICENSE

namespace QuantizeUtils {

	//copied & fixed these scales http://www.grantmuller.com/MidiReference/doc/midiReference/ScaleReference.html
	//more scales http://lawriecape.co.uk/theblog/index.php/archives/881
	int SCALE_AEOLIAN        [8] = {0, 2, 3, 5, 7, 8, 10, 12};
	int SCALE_BLUES          [7] = {0, 3, 5, 6, 7, 10, 12}; //FIXED!
	int SCALE_CHROMATIC      [13]= {0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12};
	int SCALE_DIATONIC_MINOR [8] = {0, 2, 3, 5, 7, 8, 10, 12};
	int SCALE_DORIAN         [8] = {0, 2, 3, 5, 7, 9, 10, 12};
	int SCALE_HARMONIC_MINOR [8] = {0, 2, 3, 5, 7, 8, 11, 12};
	int SCALE_INDIAN         [8] = {0, 1, 1, 4, 5, 8, 10, 12};
	int SCALE_LOCRIAN        [8] = {0, 1, 3, 5, 6, 8, 10, 12};
	int SCALE_LYDIAN         [8] = {0, 2, 4, 6, 7, 9, 11, 12};
	int SCALE_MAJOR          [8] = {0, 2, 4, 5, 7, 9, 11, 12};
	int SCALE_MELODIC_MINOR  [10] = {0, 2, 3, 5, 7, 8, 9, 10, 11, 12};
	int SCALE_MINOR          [8] = {0, 2, 3, 5, 7, 8, 10, 12};
	int SCALE_MIXOLYDIAN     [8] = {0, 2, 4, 5, 7, 9, 10, 12};
	int SCALE_NATURAL_MINOR  [8] = {0, 2, 3, 5, 7, 8, 10, 12};
	int SCALE_PENTATONIC     [6] = {0, 2, 4, 7, 9, 12};
	int SCALE_PHRYGIAN       [8] = {0, 1, 3, 5, 7, 8, 10, 12};
	int SCALE_TURKISH        [8] = {0, 1, 3, 5, 7, 10, 11, 12};

	enum NoteEnum {
		NOTE_C, 
		NOTE_C_SHARP,
		NOTE_D,
		NOTE_D_SHARP,
		NOTE_E,
		NOTE_F,
		NOTE_F_SHARP,
		NOTE_G,
		NOTE_G_SHARP,
		NOTE_A,
		NOTE_A_SHARP,
		NOTE_B,
		NUM_NOTES
	};

	enum ScaleEnum {
		AEOLIAN,
		BLUES,
		CHROMATIC,
		DIATONIC_MINOR,
		DORIAN,
		HARMONIC_MINOR,
		INDIAN,
		LOCRIAN,
		LYDIAN,
		MAJOR,
		MELODIC_MINOR,
		MINOR,
		MIXOLYDIAN,
		NATURAL_MINOR,
		PENTATONIC,
		PHRYGIAN,
		TURKISH,
		NONE,
		NUM_SCALES
	};

	// long printIter = 0;
	float closestVoltageInScale(float voltsIn, 
								int rootNote, 
								int currScale){
		int *curScaleArr;
		int notesInScale = 0;
		switch(currScale){
			case AEOLIAN:        curScaleArr = SCALE_AEOLIAN;       notesInScale=sizeof(SCALE_AEOLIAN); break;
			case BLUES:          curScaleArr = SCALE_BLUES;         notesInScale=sizeof(SCALE_BLUES); break;
			case CHROMATIC:      curScaleArr = SCALE_CHROMATIC;     notesInScale=sizeof(SCALE_CHROMATIC); break;
			case DIATONIC_MINOR: curScaleArr = SCALE_DIATONIC_MINOR;notesInScale=sizeof(SCALE_DIATONIC_MINOR); break;
			case DORIAN:         curScaleArr = SCALE_DORIAN;        notesInScale=sizeof(SCALE_DORIAN); break;
			case HARMONIC_MINOR: curScaleArr = SCALE_HARMONIC_MINOR;notesInScale=sizeof(SCALE_HARMONIC_MINOR); break;
			case INDIAN:         curScaleArr = SCALE_INDIAN;        notesInScale=sizeof(SCALE_INDIAN); break;
			case LOCRIAN:        curScaleArr = SCALE_LOCRIAN;       notesInScale=sizeof(SCALE_LOCRIAN); break;
			case LYDIAN:         curScaleArr = SCALE_LYDIAN;        notesInScale=sizeof(SCALE_LYDIAN); break;
			case MAJOR:          curScaleArr = SCALE_MAJOR;         notesInScale=sizeof(SCALE_MAJOR); break;
			case MELODIC_MINOR:  curScaleArr = SCALE_MELODIC_MINOR; notesInScale=sizeof(SCALE_MELODIC_MINOR); break;
			case MINOR:          curScaleArr = SCALE_MINOR;         notesInScale=sizeof(SCALE_MINOR); break;
			case MIXOLYDIAN:     curScaleArr = SCALE_MIXOLYDIAN;    notesInScale=sizeof(SCALE_MIXOLYDIAN); break;
			case NATURAL_MINOR:  curScaleArr = SCALE_NATURAL_MINOR; notesInScale=sizeof(SCALE_NATURAL_MINOR); break;
			case PENTATONIC:     curScaleArr = SCALE_PENTATONIC;    notesInScale=sizeof(SCALE_PENTATONIC); break;
			case PHRYGIAN:       curScaleArr = SCALE_PHRYGIAN;      notesInScale=sizeof(SCALE_PHRYGIAN); break;
			case TURKISH:        curScaleArr = SCALE_TURKISH;       notesInScale=sizeof(SCALE_TURKISH); break;
			case NONE:           return voltsIn;
		}

		//C1 == -2.00, C2 == -1.00, C3 == 0.00, C4 == 1.00
		//B1 == -1.08, B2 == -0.08, B3 == 0.92, B4 == 1.92
		float closestVal = 10.0;
		float closestDist = 10.0;
		float scaleNoteInVolts = 0;
		float distAway = 0;
		int octaveInVolts = int(floorf(voltsIn));
		float voltMinusOct = voltsIn - octaveInVolts;
		for (int i=0; i < notesInScale; i++) {
			scaleNoteInVolts = curScaleArr[i] / 12.0;
			distAway = fabs(voltMinusOct - scaleNoteInVolts);
			if(distAway < closestDist){
				closestVal = scaleNoteInVolts;
				closestDist = distAway;
			}

			// if(printIter%100000==0){
			// 	printf("i:%i, rootNote:%i, voltsIn:%f, octaveInVolts:%i, closestVal:%f, closestDist:%f\n", 
			// 	        i,    rootNote,    voltsIn,    octaveInVolts,    closestVal,    closestDist);
			// 	printIter = 0;
			// }
		}
		// printIter++;
		float voltsOut = octaveInVolts + rootNote/12.0 + closestVal;
		// if(printIter%100000==0){
		// 	printf("voltsIn:%f, voltsOut:%f, closestVal:%f\n", 
		// 			voltsIn,    voltsOut,    closestVal);
		// 	printIter = 0;
		// }
		return voltsOut;
	}


	std::string noteName(int note) {
		switch(note){
			case NOTE_C:       return "C";
			case NOTE_C_SHARP: return "C#";
			case NOTE_D:       return "D";
			case NOTE_D_SHARP: return "D#";
			case NOTE_E:       return "E";
			case NOTE_F:       return "F";
			case NOTE_F_SHARP: return "F#";
			case NOTE_G:       return "G";
			case NOTE_G_SHARP: return "G#";
			case NOTE_A:       return "A";
			case NOTE_A_SHARP: return "A#";
			case NOTE_B:       return "B";
			default: return "";
		}
	}

	std::string scaleName(int scale) {
		switch(scale){
			case AEOLIAN:        return "Aeolian";
			case BLUES:          return "Blues";
			case CHROMATIC:      return "Chromat";
			case DIATONIC_MINOR: return "Diat Min";
			case DORIAN:         return "Dorian";
			case HARMONIC_MINOR: return "Harm Min";
			case INDIAN:         return "Indian";
			case LOCRIAN:        return "Locrian";
			case LYDIAN:         return "Lydian";
			case MAJOR:          return "Major";
			case MELODIC_MINOR:  return "Mel Min";
			case MINOR:          return "Minor";
			case MIXOLYDIAN:     return "Mixolyd";
			case NATURAL_MINOR:  return "Nat Min";
			case PENTATONIC:     return "Penta";
			case PHRYGIAN:       return "Phrygian";
			case TURKISH:        return "Turkish";
			case NONE:           return "None";
			default: return "";
		}
	}

	inline float rescalefjw(float x, float xMin, float xMax, float yMin, float yMax)
	{
		return yMin + (x - xMin) / (xMax - xMin) * (yMax - yMin);
	}

	inline bool AlmostEqualRelative(float A, float B,
						 float maxRelDiff = 1.0 / 12)
	{
		// Calculate the difference.
		float diff = fabs(A - B);
		A = fabs(A);
		B = fabs(B);
		// Find the largest
		float largest = (B > A) ? B : A;

		if (diff <= largest * maxRelDiff)
			return true;
		return false;
	}
};
