// Copy this file to secrets.h (same folder as the sketch) and replace the four numbers with the ID of your card.
// secrets.h is listed in .gitignore, so your real card ID is never committed.
//
// To find the ID of a card: open the MFRC522 library example "DumpInfo" (File > Examples > MFRC522), upload it,
// hold the card to the reader and read "Card UID" in the Serial Monitor, for example "5B FD 4D 0C".
// Write each pair as 0x5B, 0xFD, 0x4D, 0x0C.
//
// Note: a card ID can be copied by anyone who can read the card, so this is a simple lock for a school project,
// not strong security.
const byte AUTHORIZED_UID[4] = {0x00, 0x00, 0x00, 0x00};
