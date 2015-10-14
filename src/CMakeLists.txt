project(NetroidsPrototype)

list(APPEND CMAKE_CXX_FLAGS "-std=c++1y -g")

set(NETROIDS_PROTOTYPE
	AABB.h
	Bullet.cpp
	Bullet.h
	Camera.cpp
	Camera.h
	ClientPlayer.cpp
	ClientPlayer.h
	Entity.h
	GameClient.cpp
	GameClient.h
	GameEngine.cpp
	GameEngine.h
	GameHost.cpp
	GameHost.h
	GameServer.cpp
	GameServer.h
	GameState.h
	Input.cpp
	Input.h
	MoveHistory.cpp
	MoveHistory.h
	Net.pb.cpp
	Net.pb.h
	Net.proto
	Networking.cpp
	Networking.h
	Player.cpp
	Player.h
	Rendering.cpp
	Rendering.h
	ServerPlayer.cpp
	ServerPlayer.h
	Ship.cpp
	Ship.h
	StateHostGame.cpp
	StateHostGame.h
	StateRemoteGame.cpp
	StateRemoteGame.h
	main.cpp
	test.frag
	test.vert)

add_executable(NetroidsPrototype ${NETROIDS_PROTOTYPE})
target_link_libraries(NetroidsPrototype SDL2)
target_link_libraries(NetroidsPrototype GL)
target_link_libraries(NetroidsPrototype GLEW)
target_link_libraries(NetroidsPrototype enet)
target_link_libraries(NetroidsPrototype protobuf)
