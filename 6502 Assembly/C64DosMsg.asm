// C64DosMsg: changes display colors to those of MS-DOS and prints a string 
// Author: Jacob Niemeir
// Written for C64 with Kick Assembler syntax in mind
*=832			    // Set starting memory address to be within program storage area	
    jsr $e518               // Clear screen
    lda #0                  // Load color black to accumulator
    sta $D020               // Set border color to value in accumulator
    sta $D021               // Set background color to value in accumulator
    lda #1                  // Load color white to accumulator
    sta $0286               // Set text color to value in accumulator
    ldx #$00                // Load first row into x register 
    ldy #$00                // Load first column into y register
    jsr $e50c               // Set cursor position

print:   		    // Labels like this one are used to mark locations in memory
    lda #<str               // Load low byte (the lower 8 bits) of string address into accumulator
    ldy #>str               // Load high byte (the upper 8 bits) of string address into y register
    jsr $ab1e               // Print null terminated string
    rts                     // Return from subroutine

str:    		    
    .text "STARTING MS-DOS..." // The string is stored into the memory location indicated by str
    .byte $00	// A null terminator is added immediately after string

