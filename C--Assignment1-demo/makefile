
COMPILER = g++

CFLAGS = -Wall -Werror -std=c++17


towerstest:
	@$(COMPILER)  $(CFLAGS) towers/towers.cpp towers/towers_test_driver.cpp -o towers_test
	@./towers_test
	@rm towers_test

crittertest:
	@$(COMPILER) $(CFLAGS) Critter/critter.cpp -o critter_test


maptest:
	$(COMPILER) $(CFLAGS) Map/Map.cpp towers/towers.cpp Critter/critter.cpp  main.cpp -o mapdriver
	@./mapdriver
	@rm mapdriver

