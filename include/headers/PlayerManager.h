typedef struct playerManagerInstance PlayerManagerInstance;

typedef struct playerManager
{
	void (*init)(void *self); // init array with all player alive
	int (*winner)(void *self); //-1 for no winner, else ID of winner
	void (*killed)(void *self, int id); // set alive to false for specific id

	PlayerManagerInstance *instance;
}PlayerManager;

PlayerManager *getPlayerManager();