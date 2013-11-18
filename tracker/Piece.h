#ifndef TORRENTIAL_DOWNLOADS_PIECE
#define TORRENTIAL_DOWNLOADS_PIECE

/* Class that holds information on a file piece.*/
class Piece {

	private:
		/* Array of bytes that hold the has of this file piece. */
		uint8_t * pieceHash;

		/* The length of the byte array, pieceHash. */
		uint32_t pieceLength;

		/* The size of the file piece in bytes. */
		uint32_t pieceSize;

	public:
		/* Takes a uint8_t array of size 20. Sets it in this piece. 
		   Also takes a newPieceSize which is the size of this file piece in bytes. */
		Piece (uint8_t * newPieceHash, uint32_t newPieceSize) {

			pieceHash = new uint8_t[20];
			pieceLength = 20;
			pieceSize = newPieceSize;

			for (uint32_t i = 0; i < 20; i++) {

				pieceHash[i] = newPieceHash[i];
			}
		}

		/* Takes a uint8_t array of size, newPieceLength. Sets it, and the size, in this piece. 
		   Also takes a newPieceSize which is the size of this file piece in bytes. */
		Piece(uint8_t * newPieceHash, uint32_t newPieceLength, uint32_t newPieceSize) {

			pieceHash = new uint8_t[newPieceLength];
			pieceLength = newPieceLength;
			pieceSize = newPieceSize;

			for (uint32_t i = 0; i < newPieceLength; i++) {

				pieceHash[i] = newPieceHash[i];
			}	
		}

		const uint8_t * getPieceHash() const {

			return pieceHash;
		}

		const uint32_t getPieceLength() const {

			return pieceLength;
		}

		const uint32_t getPieceSize() const {

			return pieceSize;
		}
};

struct PieceHash {

	std::size_t operator()(const Piece & p) const {

		size_t hash = 51 + (std::hash<uint32_t>()(p.getPieceLength()) ^ std::hash<uint32_t>()(p.getPieceSize()));
		size_t byteHash;
		for (uint32_t i = 0; i < p.getPieceLength(); i++) {

			byteHash = p.getPieceHash()[i] >> (p.getPieceLength() + 1 - i);
			hash ^= byteHash;
		}

		return hash;
	}
};

#endif