ODIR = obj
SDIR = src

HEADERS = $(SDIR)/ast.hpp $(SDIR)/lexer.hpp $(SDIR)/parser.hpp
LIBS = $(ODIR)/ast.o $(ODIR)/lexer.o $(ODIR)/parser.o
FLAGS = -g -O3 `llvm-config --cxxflags --ldflags --system-libs --libs core`

kaleidoscope : $(SDIR)/kaleidoscope.cpp $(LIBS) $(HEADERS)
	@echo 'Compiling [kaleidoscope.cpp]'
	@clang++ $(SDIR)/kaleidoscope.cpp -o kaleidoscope $(LIBS) $(FLAGS)

$(ODIR)/ast.o : $(SDIR)/ast.cpp $(SDIR)/ast.hpp
	@echo 'Compiling [ast.cpp]'
	@cd $(SDIR) && clang++ ast.cpp -c -g -O3 -o ../obj/ast.o && cd ..

$(ODIR)/lexer.o : $(SDIR)/lexer.cpp $(SDIR)/lexer.hpp
	@echo 'Compiling [lexer.cpp]'
	@cd $(SDIR) && clang++ lexer.cpp -c -g -O3 -o ../obj/lexer.o && cd ..

$(ODIR)/parser.o : $(SDIR)/parser.cpp $(SDIR)/ast.hpp $(SDIR)/lexer.hpp $(SDIR)/parser.hpp
	@echo 'Compiling [parser.cpp]'
	@cd $(SDIR) && clang++ parser.cpp -c -g -O3 -o ../obj/parser.o && cd ..

clean :
	@echo 'Removing executable and object files'
	@rm -f kaleidoscope obj/*.o
