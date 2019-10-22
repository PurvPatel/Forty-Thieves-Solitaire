/**
 * \file CardTypes.h
 * \author Maninder Sran
 * \brief
 */
#ifndef A3_CARD_TYPES_H_
#define A3_CARD_TYPES_H_

/**
 * \brief Describes the rank of a card.
 */
typedef unsigned short int RankT;

/**
 * \brief RankT for an Ace.
 */
#define ACE    1

/**
 * \brief RankT for an Jack.
 */
#define JACK   11

/**
 * \brief RankT for a Queen.
 */
#define QUEEN  12

/**
 * \brief RankT for a King.
 */
#define KING   13

/**
 * \brief Suit types for CardT.
 */
enum SuitT {HEART, DIAMOND, CLUB, SPADE};

/**
 * \brief Category types for CardStack
 */
enum CategoryT {Tableau, Foundation, Deck, Waste};

/**
 * \brief Tuple of type CardT.
 */
struct CardT {
	SuitT s;
	RankT r;
};

#endif