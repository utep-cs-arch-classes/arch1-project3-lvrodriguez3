	.arch			msp430g2553
	.p2align		1,0

song_1: .string "z"
song_2: .string "ddd"
song_3: .string "eee"
song_4: .string "cccccc dddddd eeeeee ffffff gggggg aaaaaa bbbbbb CCCCCC"

	.globl songs
songs:  .word song_1 
	.word song_2
	.word song_3
	.word song_4

	.weak current_song
	.weak current_note

	.text

	.globl advance_song
advance_song:

	; void advance_song() {
	;   if(songs[current_song][current_note]) {
	;     current_note++;
	;   }
	; }

	mov #songs, r15 	
	add &current_song, r15
	add &current_song, r15	
	mov 0(r15), r15		
	add &current_note, r15	
	cmp.b #0, 0(r15)	
	jz end			
	add #1, &current_note	
end:	ret		
