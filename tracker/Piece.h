#ifndef TORRENTIAL_DOWNLOADS_PIECE
#define TORRENTIAL_DOWNLOADS_PIECE

/* Class that holds information on a file piece.*/
class Piece {

	private:
		/* Array of bytes that hold the hash of this file piece. */
		uint8_t * pieceHash;

		/* The length of the byte array, pieceHash. */
		uint32_t pieceLength;

		/* The size of the file piece in bytes. */
		uint32_t pieceSize;

		/* The index of the piece amongst its other pieces. */
		uint32_t pieceIndex;

	public:
		/* Takes a uint8_t array of size 20. Sets it in this piece. 
		   An index indicating this piece's position amongst all of the pieces.
		   Also takes a newPieceSize which is the size of this file piece in bytes. */
		Piece (uint8_t * newPieceHash, uint32_t newIndex, uint32_t newPieceSize) {

			pieceHash = new uint8_t[20];
			pieceIndex = newIndex;
			pieceLength = 20;
			pieceSize = newPieceSize;

			for (uint32_t i = 0; i < 20; i++) {

				pieceHash[i] = newPieceHash[i];
			}
		}

		/* Takes a uint8_t array of size, newPieceLength. Sets it, and the size, in this piece. 
		   An index indicating this piece's position amongst all of the pieces.
		   Also takes a newPieceSize which is the size of this file piece in bytes. */
		Piece(uint8_t * newPieceHash, uint32_t newIndex, uint32_t newPieceLength, uint32_t newPieceSize) {

			pieceHash = new uint8_t[newPieceLength];
			pieceIndex = newIndex;
			pieceLength = newPieceLength;
			pieceSize = newPieceSize;

			for (uint32_t i = 0; i < newPieceLength; i++) {

				pieceHash[i] = newPieceHash[i];
			}	
		}

		/* Takes the index, and the size of the piece in bytes. 
		   Used to create a Piece to find a piece in the pieces list. */
		Piece (uint32_t newIndex, uint32_t newPieceSize) {

			pieceIndex = newIndex;
			pieceSize = newPieceSize;
			pieceHash = NULL;
			pieceLength = 20;
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

		const uint32_t getPieceIndex() const {

			return pieceIndex;
		}

		bool operator== (const Piece & p) const {

			if (p.getPieceIndex() == pieceIndex 
				&& p.getPieceSize() == pieceSize
				&& p.getPieceLength() == pieceLength) {

				return true;
			}

			return false;
		}
};

struct PieceHash {

	std::size_t operator()(const Piece & p) const {

		size_t hash = 51 + (std::hash<uint32_t>()(p.getPieceLength()) 
			^ std::hash<uint32_t>()(p.getPieceSize()) 
			^ std::hash<uint32_t>()(p.getPieceIndex()));

		return hash;
	}
};

#endif