template<typename F> void ABoard::mapEntities(F fun) {
	for(auto it = entities.begin(); it != entities.end(); it++) {
		fun(*it);
	}
}

template<typename Pred>
std::vector<std::shared_ptr<Entity>> ABoard::selectEntities(Pred pred) {
	std::vector<std::shared_ptr<Entity>> ret;
	mapEntities([&ret, pred] (std::shared_ptr<Entity> e) {if (pred(e)) {ret.push_back(e);};});
	return ret;
}

