#include <iostream>
#include <raylib.h>
#include <deque>
#include <raymath.h>

using namespace std;

enum Colors{REDc, GREENc, PURPLEc};

Color green = {100, 100, 100, 255};
Color Darkgreen = {100, 210, 100, 255};

int cellSize = 30;
int cellCount = 25;
int offset = 75;

double lastUpdateTime = 0;

bool checkElement(Vector2 element, deque<Vector2> deque)
{
    for(unsigned int i = 0; i < deque.size(); i++)
    {
        if (Vector2Equals(deque[i], element))
        {
            return true;
        }
        
    }
    return false;
}


bool eventTriggered(double interval)
{
    double currentTime = GetTime();
    if (currentTime - lastUpdateTime >= interval)
    {
        lastUpdateTime = currentTime;
        return true;
    }
    return false;
}

class Food{

protected:
    Vector2 position;
    Texture2D texture;
public:
    Food(deque<Vector2> snakeBody)
    {
        Image image = LoadImage("Graphics/cross.png");
        texture = LoadTextureFromImage(image);
        UnloadImage(image);
        position = GenerateRandomPos(snakeBody);
    }

    Food(deque<Vector2> snakeBody, const char* imagePath)
    {
        Image image = LoadImage(imagePath);
        texture = LoadTextureFromImage(image);
        UnloadImage(image);
        position = GenerateRandomPos(snakeBody);
    }

    ~Food()
    {   
        UnloadTexture(texture);
    }

    virtual void Draw() = 0;

    Vector2 GenerateRandomCell()
    {
        float x = GetRandomValue(0, cellCount - 1);
        float y = GetRandomValue(0, cellCount - 1);
        return Vector2{x,y};
    }

    Vector2 GenerateRandomPos(deque<Vector2> snakeBody)
    {
        Vector2 position = GenerateRandomCell();

        while(checkElement(position, snakeBody))
        {
            position = GenerateRandomCell();
        }

        return position;
    }

    Vector2 GetPosition() {
        return position;
    }

    void SetPosition(const Vector2& newPosition) {
        position = newPosition;
    }
};

class GreenFood : public Food
{
private:
    int color = GREENc;
public:
    GreenFood() : Food({})
    {}
    
    GreenFood(deque<Vector2> snakeBody, const char* imagePath) : Food(snakeBody, imagePath) {}

    void Draw() override 
    {
        DrawTexture(texture, offset + position.x * cellSize, offset + position.y * cellSize, GREEN);
    }
};

class RedFood : public  Food
{
private:
    int color = REDc;
public:
    RedFood() : Food({})
    {
        SetPosition(GenerateRandomPos({}));
    }

    RedFood(deque<Vector2> snakeBody, const char* imagePath) : Food(snakeBody, imagePath) {}

    void Draw() override 
    {
        DrawTexture(texture, offset + position.x * cellSize, offset + position.y * cellSize, RED);
    }
};

class PurpleFood : public  Food
{
private:
    int color = PURPLEc;
public:
    PurpleFood() : Food({})
    {
        SetPosition(GenerateRandomPos({}));
    }

    PurpleFood(deque<Vector2> snakeBody, const char* imagePath) : Food(snakeBody, imagePath) {}

    void Draw() override 
    {
        DrawTexture(texture, offset + position.x * cellSize, offset + position.y * cellSize, PURPLE);
    }
};

class Snake
{
private:
    deque <Vector2> body;
    Vector2 direction;
    bool addPart;
public:
    Snake() : body({Vector2{6, 9}, Vector2{5, 9}, Vector2{4, 9}}), direction({1,0}), addPart(false) {}

    void Draw()
    {
        for (unsigned int i = 0; i < body.size(); i++)
        {
            float x = body[i].x;
            float y = body[i].y;
            Rectangle part = Rectangle{offset + x*cellSize, offset + y*cellSize,(float)cellSize, (float)cellSize};
            DrawRectangleRounded(part, 0.5, 6, Darkgreen);
        }
    }

    void UpdateBody()
    {
        body.push_front(Vector2Add(body[0], direction));
        if (addPart)
        {
            addPart = false;
        } else
        {
            body.pop_back();
        }
    }

    void Reset()
    {
        body = {Vector2{6, 9}, Vector2{5, 9}, Vector2{4, 9}};
    }

    deque<Vector2>& GetBody() {
        return body;
    }

    bool GetAddPart() {
        return addPart;
    }
    
    void SetAddPart(bool value) {
        addPart = value;
    }

   Vector2& GetDirection() {
        return direction;
    }
    
    void SetDirection(const Vector2& newDirection) {
        direction = newDirection;
    }
};


class Game
{
private:
    Snake snake = Snake();
    Food* food = nullptr;
    int previousColor = -1;
    bool running = true;
    const char* imagePaths[3] = {"Graphics/cross.png", "Graphics/mark.png", "Graphics/rhombus.png"};
    int score = 0;
public:
    Game()
    {
        food = new GreenFood();
    }

    Snake& GetSnake() { return snake; }

    void SpawnRandomFood() {
        int randomColor = GetRandomValue(0, 2);
        int randomPathIndex = GetRandomValue(0, 2);

        if (food != nullptr) {
            food = nullptr;
        }

        if (randomColor == GREENc) {
            if (randomColor == previousColor)
            {
                food = new GreenFood();
            }
        else {    
            previousColor = randomColor;
            food = new GreenFood(snake.GetBody(), imagePaths[randomPathIndex]);
        }
        } else if (randomColor == REDc) {
            if (randomColor == previousColor)
            {
                food = new RedFood();
            }
        else {
            previousColor = randomColor;    
            food = new RedFood(snake.GetBody(), imagePaths[randomPathIndex]);
        }
            
        } else {
            if (randomColor == previousColor)
            {
                food = new PurpleFood();
            }
        else {    
            previousColor = randomColor;
            food = new PurpleFood(snake.GetBody(), imagePaths[randomPathIndex]);
        }
        }
    }
    void Draw()
    {
        food->Draw();
        snake.Draw();
    }

    void Update()
    {
        if (running)
        {
            snake.UpdateBody();
            checkFoodCollision(food);
            checkEdgeCollision();
            checkTailCollision();
        }
        
    }

    void checkFoodCollision(Food* currentFood)
    {
        deque<Vector2> body = snake.GetBody();
        if(Vector2Equals(body[0], currentFood->GetPosition()))
        {
            SpawnRandomFood();
            snake.SetAddPart(true);
            score++;
        }
    }

    void checkEdgeCollision()
    {
        if(snake.GetBody()[0].x == cellCount || snake.GetBody()[0].x == -1)
        {
            GameOver();
        }
        if(snake.GetBody()[0].y == cellCount || snake.GetBody()[0].y == -1)
        {
            GameOver();
        }
    }

    void checkTailCollision()
    {
        deque<Vector2> bodyWithoutHead = snake.GetBody();
        bodyWithoutHead.pop_front();
        if(checkElement(snake.GetBody()[0], bodyWithoutHead))
        {
            GameOver();
        }
    }

    void GameOver()
    {
        snake.Reset();
        food->SetPosition(food->GenerateRandomPos(snake.GetBody()));
        running = false;
        score = 0;
    }

    bool GetRunning() {
        return running;
    }

    void SetRunning(bool value) {
        running = value;
    }

    int GetScore() {
        return score;
    }
};

int main()
{
    InitWindow(offset*2 + cellSize * cellCount, offset*2 + cellSize*cellCount, "Snake");
    SetTargetFPS(60);

    Game game = Game();

    while(!WindowShouldClose())
    {
        BeginDrawing();

        if (eventTriggered(0.2))
        {
            game.Update();
        }
        
        if(IsKeyPressed(KEY_UP) && game.GetSnake().GetDirection().y != 1)
        {
            game.GetSnake().SetDirection({0, -1});
            game.SetRunning(true);
        }

        if(IsKeyPressed(KEY_DOWN) && game.GetSnake().GetDirection().y != -1)
        {
            game.GetSnake().SetDirection({0, 1});
            game.SetRunning(true);   
        }

        if(IsKeyPressed(KEY_LEFT) && game.GetSnake().GetDirection().x != 1)
        {
            game.GetSnake().SetDirection({-1, 0});   
            game.SetRunning(true);
        }

        if(IsKeyPressed(KEY_RIGHT) && game.GetSnake().GetDirection().x != -1)
        {
            game.GetSnake().SetDirection({1, 0});  
            game.SetRunning(true); 
        }        

        ClearBackground(green);
        DrawRectangleLinesEx(Rectangle{(float)offset - 5, (float)offset - 5, (float)cellSize*cellCount + 10,  (float)cellSize*cellCount +10}, 5, Darkgreen);
        DrawText(TextFormat("%i", game.GetScore()), offset - 5, offset + cellSize*cellCount + 10, 40, Darkgreen);
        game.Draw();

        EndDrawing();
    }


    CloseWindow();
    return 0;
}