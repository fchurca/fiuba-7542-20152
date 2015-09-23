template<typename Pred>
std::vector<std::shared_ptr<Entity>> Board::selectEntities(Pred pred) {
	std::vector<std::shared_ptr<Entity>> ret;
	for(auto it = entities.begin(); it != entities.end(); it++) {
		if (pred(*it)) {
			ret.push_back(*it);
		}
	}
	return ret;
}

