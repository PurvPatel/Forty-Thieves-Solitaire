#ifndef A3_BOARD_H_
#define A3_BOARD_H_

#include "CardTypes.h"
#include "CardStack.h"
#include "GameBoard.h"
#include <iostream>

BoardT::BoardT(std::vector<CardT> deck){
	std::vector<CardT> temp;
	if(!two_decks(init_seq(10),init_seq(8),CardStackT(deck),CardStackT(temp))){
		throw std::invalid_argument("Invalid Here");
	}

	T = tab_deck(deck);
	F = init_seq(8);
	
	for(int i = 40; i < 103; i++){
		D.push(deck[i]);
	}

	W = CardStackT(temp);
}

bool BoardT::is_valid_tab_mv(CategoryT c, unsigned int n0, unsigned int n1){
	if(c == Tableau){
		if(!(is_valid_pos(Tableau,n0) && is_valid_pos(Tableau,n1))){			
			throw std::out_of_range("Invalid Move");
		}
		return valid_tab_tab(n0,n1);
	}
	else if(c == Foundation){
		if(!(is_valid_pos(Tableau,n0) && is_valid_pos(Foundation,n1))){			
			throw std::out_of_range("Invalid Move");
		}
		return valid_tab_foundation(n0,n1);
	}
	else if(c == Deck){
		return false;
	}
	return false;
}

bool BoardT::is_valid_waste_mv(CategoryT c, unsigned int n){
	if(W.size() == 0){
		throw std::invalid_argument("No cards in the waste");
	}
	if(c == Tableau){
		if(!is_valid_pos(Tableau,n)){			
			throw std::out_of_range("Invalid Move");
		}
		return valid_waste_tab(n);
	}
	else if(c == Foundation){
		if(!is_valid_pos(Foundation,n)){			
			throw std::out_of_range("Invalid Move");
		}
		return valid_waste_foundation(n);
	}
	else if(c == Deck){
		return false;
	}
	return false;
}

bool BoardT::is_valid_deck_mv(){
	return D.size() > 0;
}

void BoardT::tab_mv(CategoryT c, unsigned int n0, unsigned int n1){
	if(!is_valid_tab_mv(c,n0,n1)){
		std::cout << "test" << std::endl;
		throw std::invalid_argument("Invalid Move");
	}

	if(c == Tableau){
		T[n1] = T[n1].push(T[n0].top());
		T[n0] = T[n0].pop();
	}
	else if(c == Foundation){
		F[n1] = F[n1].push(T[n0].top());
		T[n0] = T[n0].pop();
	}
}

void BoardT::waste_mv(CategoryT c, unsigned int n){
	if(!is_valid_waste_mv(c,n)){
		throw std::invalid_argument("Stack is empty");	
	}

	if(c == Tableau){
		T[n] = T[n].push(W.top());
		W = W.pop();
	}
	else if(c == Foundation){
		F[n] = F[n].push(W.top());
		W = W.pop();
	}
}

void BoardT::deck_mv(){
	if(!is_valid_deck_mv()){
		throw std::invalid_argument("Deck is empty");		
	}
	W = W.push(D.top());
	D = D.pop();
}

CardStackT BoardT::get_tab(unsigned int i){
	if(!is_valid_pos(Tableau,i)){
		throw std::out_of_range("Stack is empty");	
	}
	return F[i];
}

CardStackT BoardT::get_foundation(unsigned int i){
	if(!is_valid_pos(Foundation,i)){
		throw std::out_of_range("Stack is empty");
	}
	return F[i];
}

CardStackT BoardT::get_deck(){
	return D;
}

CardStackT BoardT::get_waste(){
	return W;
}

bool BoardT::valid_mv_exists(){
	if(is_valid_deck_mv()){
		return true;
	}
	for(unsigned int c = 0; c < 2; c++){
		for(unsigned int n0 = 0; n0 < 10; n0++){
			for(unsigned int n1 = 0; n1 < 8; n1++){
				if(is_valid_pos(Tableau,n0) && is_valid_pos(static_cast<CategoryT>(c),n1)){
					if(is_valid_tab_mv(static_cast<CategoryT>(c),n0,n1)){
						return true;
					}
				}
			}
		}
	}

	for(unsigned int c = 0; c < 2; c++){
		for(unsigned int n = 0; n < 10; n++){
			if(is_valid_pos(static_cast<CategoryT>(c),n)){
				if(is_valid_waste_mv(static_cast<CategoryT>(c),n)){
					return true;
				}
			}
		}
	}	
	return false;
}

bool BoardT::is_win_state(){
	for(unsigned int i = 0; i <= 7; i++){
		if(!(F[i].size() > 0 && F[i].top().r == KING)){
			return false;
		}
	}
	return true;
}

bool BoardT::two_decks(SeqCrdStckT T,SeqCrdStckT F,CardStackT D, CardStackT W){
	for(RankT rank = ACE; rank <= KING; rank++){
		for(unsigned int suit = 0; suit < 4; suit++){
			std::function<bool(CardT)> func = [suit, rank] (CardT c) {
				return c.s == suit && c.r == rank;
			};
			if(cnt_cards(T,F,D,W,func) != 2){
				return false;
			}
		}
	}
	return true;
}

unsigned int BoardT::cnt_cards_seq(SeqCrdStckT S, std::function<bool(CardT)> f){
	unsigned int cardCount = 0;
	for(unsigned int i = 0; i < S.size(); i++){
		cardCount += cnt_cards_stack(S[i],f);
	}
	return cardCount;
}

unsigned int BoardT::cnt_cards_stack(CardStackT s, std::function<bool(CardT)> f){
	unsigned int cardCount = 0;
	std::vector<CardT> stack = s.toSeq();
	for(unsigned int i = 0; i < stack.size(); i++){
		if(f(stack[i])){
			cardCount++;
		}
	}
	return cardCount;
}

unsigned int BoardT::cnt_cards(SeqCrdStckT T, SeqCrdStckT F, CardStackT D, CardStackT W, std::function<bool(CardT)> f){
	return cnt_cards_seq(T,f)+cnt_cards_seq(F,f)+cnt_cards_stack(D,f)+cnt_cards_stack(W,f);
}

SeqCrdStckT BoardT::init_seq(unsigned int n){
	SeqCrdStckT s;
	for(unsigned int i = 0; i < n; i++){
		std::vector<CardT> S;
		CardStackT temp(S);
		s.push_back(temp);
	}
	return s;
}

SeqCrdStckT BoardT::tab_deck(std::vector<CardT> deck){
	SeqCrdStckT T; 
	for(unsigned int i = 0; i <= 9; i++){
		std::vector<CardT> s;
		CardStackT temp(s);
		for(unsigned int j = 4*i; j <= 4*(i+1)-1; j++){
			temp.push(deck.at(j));
		}
		T.push_back(temp);
	}
	return T;
}

bool BoardT::is_valid_pos(CategoryT c, unsigned int n){
	if(c == Tableau){
		return (n >= 0 && n <= 9);
	}
	else if(c == Foundation){
		return (n >= 0 && n <= 7);
	}
	return true;
}

bool BoardT::valid_tab_tab(unsigned int n0, unsigned int n1){
	if(T[n0].size() > 0){
		if(T[n1].size() > 0){
			return tab_placeable(T[n0].top(),T[n1].top());
		}
		else{
			return true;
		}
	}
	else{
		if(T[n1].size() > 0){
			return false;
		}
		else{
			return false;
		}	
	}
}

bool BoardT::valid_tab_foundation(unsigned int n0, unsigned int n1){
	if(T[n0].size() > 0){
		if(F[n1].size() > 0){
			return foundation_placeable(T[n0].top(), F[n1].top());
		}
		else{
			return T[n0].top().r == ACE;
		}
	}
	else{
		if(F[n1].size() > 0){
			return false;
		}
		else{
			return false;
		}	
	}
}

bool BoardT::valid_waste_tab(unsigned int n){
	if(T[n].size() > 0){
		return tab_placeable(W.top(), T[n].top());
	}
	return true;
}

bool BoardT::valid_waste_foundation(unsigned int n){
	if(T[n].size() > 0) {
		return foundation_placeable(W.top(),F[n].top());
	}
	return W.top().r == ACE;
}

bool BoardT::tab_placeable(CardT c, CardT d){
	std::cout << (c.s == d.s && c.r == (d.r - 1)) << std::endl;
	return (c.s == d.s && c.r == (d.r - 1));
}

bool BoardT::foundation_placeable(CardT c, CardT d){
	return (c.s == d.s && c.r == (d.r + 1));
}
#endif