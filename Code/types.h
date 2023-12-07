/* 
Create a data structure to represent each game. This data structure should store the id, name, average user
rating, user rating count, developer and size.
 */
struct GAME_
{
    unsigned id;
    char *name;
    float average_user_rating;
    int user_rating_count;
    char *developer;
    unsigned size;
};

class node
{
public:
    node *next, *prev;
    GAME_ *game;
    node(GAME_ *game)
    {
        next = NULL;
        prev = NULL;
        this->game = game;
    }
};