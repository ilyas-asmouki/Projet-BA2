CXX     = g++
CXXFLAGS = -g -Wall -std=c++11
CXXFILES = projet.cc simulation.cc robot.cc particule.cc message.cc shape.cc
OFILES = projet.o simulation.o robot.o particule.o message.o shape.o


projet: $(OFILES)
	$(CXX) $(OFILES) -o projet

# Definitions de cibles particulieres

depend:
	@echo " *** MISE A JOUR DES DEPENDANCES ***"
	@(sed '/^# DO NOT DELETE THIS LINE/q' Makefile && \
	  $(CXX) -MM $(CXXFLAGS) $(CXXFILES) | \
	  egrep -v "/usr/include" \
	 ) >Makefile.new
	@mv Makefile.new Makefile

clean:
	@echo " *** EFFACE MODULES OBJET ET EXECUTABLE ***"
	@/bin/rm -f *.o *.x *.cc~ *.h~ prog

#
# -- Regles de dependances generees automatiquement
#
# DO NOT DELETE THIS LINE
projet.o: projet.cc simulation.h
simulation.o: simulation.cc simulation.h robot.h shape.h particule.h \
 constantes.h message.h
robot.o: robot.cc robot.h shape.h message.h constantes.h particule.h
particule.o: particule.cc particule.h shape.h constantes.h message.h
message.o: message.cc message.h
shape.o: shape.cc shape.h
