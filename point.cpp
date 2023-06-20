class EC_point {
    private:
        int x, y;

    public:
        EC_point(int a, int b) {
            x = a;
            y = b;
        }

        int getX() {
            return x;
        }

        int getY() {
            return y;
        }

        EC_point &operator+(const EC_point a) {
            // point addition
            int lambda, newX, newY;
            lambda = (a.x-x)/(a.y-y);
            newX = lambda*lambda-x-a.x;
            newY = lambda*(x-newX)-y;
            EC_point newPoint(newX, newY);
            return newPoint;
        }

        /*
        point doubling i.e. P + P = 2P requires:
        lambda = (3x_1^2 + 2a_2x_1 - a_1y_1 + a_4) / (2y_1 + a_1x_1 + a_3)
        we denote 2P (the result) as (x_3, y_3)
        */
        EC_point doublePoint(const EC_point a) {
            
        }
};