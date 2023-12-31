#define FALSE 0
#define TRUE 1

#define WINDOW_WIDTH 1500
#define WINDOW_HEIGHT 900

#define L_GROUND_X 0
#define L_GROUND_Y WINDOW_HEIGHT-100
#define L_GROUND_W 600
#define L_GROUND_H 100

#define R_GROUND_X 850
#define R_GROUND_Y WINDOW_HEIGHT-100
#define R_GROUND_W WINDOW_WIDTH-R_GROUND_X
#define R_GROUND_H 200

#define BRIDGE_X L_GROUND_W
#define BRIDGE_Y WINDOW_HEIGHT-100
#define BRIDGE_W R_GROUND_X - L_GROUND_W
#define BRIDGE_H 20

#define L_TOWER_X 0
#define L_TOWER_Y 200
#define L_TOWER_W 250
#define L_TOWER_H L_GROUND_Y - L_TOWER_Y

#define R_TOWER_X WINDOW_WIDTH - 250
#define R_TOWER_Y 200
#define R_TOWER_W 250
#define R_TOWER_H R_GROUND_Y - R_TOWER_Y

#define STORAGE_X 50
#define STORAGE_Y 600
#define STORAGE_W 250
#define STORAGE_H 200

#define BATTERY_VELOCITY 30
#define BATTERY_X 900
#define BATTERY_Y R_GROUND_Y - 50
#define BATTERY_W 80
#define BATTERY_H 50

#define HELICOPTER_X R_TOWER_X
#define HELICOPTER_Y 50
#define HELICOPTER_W 180
#define HELICOPTER_H 90
#define HELICOPTER_VELOCITY 10

#define NUM_HOSTAGES 10
#define HOSTAGE_X L_TOWER_X
#define HOSTAGE_Y L_TOWER_Y - 20
#define HOSTAGE_W 10
#define HOSTAGE_H 20

#define NUM_SHOTS 30
#define SHOT_W 20
#define SHOT_H 20
#define SHOT_VELOCITY 25