CXX = g++
CXXFLAGS = -std=c++17 -Wall -Wextra -O3
LDFLAGS = -lm
OBJS = main.o scene.o light.o sphere.o raymarcher.o ray.o camera.o vector3.o color.o object.o plane.o cube.o torus.o cone.o cubewithhole.o mandelbulb.o frame.o

raymarcher: $(OBJS)
	$(CXX) $(CXXFLAGS) -o $@ $^ $(LDFLAGS)

main.o: main.cpp
	$(CXX) $(CXXFLAGS) -c main.cpp

scene.o: scene.cpp scene.h
	$(CXX) $(CXXFLAGS) -c scene.cpp

light.o: light.cpp light.h
	$(CXX) $(CXXFLAGS) -c light.cpp

sphere.o: sphere.cpp sphere.h
	$(CXX) $(CXXFLAGS) -c sphere.cpp

raymarcher.o: raymarcher.cpp raymarcher.h
	$(CXX) $(CXXFLAGS) -c raymarcher.cpp

ray.o: ray.cpp ray.h
	$(CXX) $(CXXFLAGS) -c ray.cpp

camera.o: camera.cpp camera.h
	$(CXX) $(CXXFLAGS) -c camera.cpp

vector3.o: vector3.cpp vector3.h
	$(CXX) $(CXXFLAGS) -c vector3.cpp

color.o: color.cpp color.h
	$(CXX) $(CXXFLAGS) -c color.cpp

object.o: object.cpp object.h
	$(CXX) $(CXXFLAGS) -c object.cpp

plane.o: plane.cpp plane.h
	$(CXX) $(CXXFLAGS) -c plane.cpp

cube.o: cube.cpp cube.h
	$(CXX) $(CXXFLAGS) -c cube.cpp

torus.o: torus.cpp torus.h
	$(CXX) $(CXXFLAGS) -c torus.cpp

cone.o: cone.cpp cone.h
	$(CXX) $(CXXFLAGS) -c cone.cpp

cubewithhole.o: cubewithhole.cpp cubewithhole.h
	$(CXX) $(CXXFLAGS) -c cubewithhole.cpp

mandelbulb.o: mandelbulb.cpp mandelbulb.h
	$(CXX) $(CXXFLAGS) -c mandelbulb.cpp

frame.o: frame.cpp frame.h
	$(CXX) $(CXXFLAGS) -c frame.cpp

clean:
	rm -f *.o raymarcher
