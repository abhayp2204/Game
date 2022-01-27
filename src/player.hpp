#ifndef PLAYER_H
#define PLAYER_H

class Player
{
public:
    // To draw the player
    std::vector<GLfloat> vertices;
    std::vector<unsigned int> indices;

    // Variables
    int score;
    bool alive;

    // Functions
    void init();
    void spawn();
    void attack();
    void defend();
};

void Player::spawn()
{
    cout << "Player spawns" << endl;
}

void Player::attack()
{
    cout << "Player kicks the enemy" << endl;
}

void Player::defend()
{
    cout << "Player uuses shields" << endl;
}

#endif