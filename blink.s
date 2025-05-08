  .org $0200

reset:
  lda #$ff
  sta $6002

  lda #$ff
  sta $6000

loop:
  ror
  sta $6000

  jmp loop
