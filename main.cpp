#include <iostream>
#include <vector>
#include <string>
#include <functional>
#include <algorithm>


//std::vector<Value *> build_topo(Value* V, std::vector<Value *> visited = {});


class Value
{
private:
    float data;

    std::vector<Value *> build_topo(Value* V, std::vector<Value *> visited = {}){
    if(std::find(visited.begin(), visited.end(), V) != visited.end()) {

    } 
    else {
        visited.push_back(V);
        for(size_t i = 0; i < V->prev.size(); i++){
            visited = build_topo(V->prev[i], visited);
        }
    }
    return visited;
    }
public:
    std::vector<Value *> prev;
    std::string op;
    float grad = 0.0;
    std::function<void()> _backward;
    Value(float val, std::vector<Value *> children = {}, std::string op = ""): data(val), prev(children), op(op){}

    float out(){
        return data;
    }

    Value operator+(Value& V){
        std::vector<Value*> children = {this, &V};
        Value out(data + V.data, children, "+");
        out._backward = [&](){
            this->grad += 1.0 * out.grad;
            V.grad += 1.0 * out.grad;
        };
        return out;
    }

    Value operator+(float& V){
        Value x(V);
        return *this + x;
    }

    Value operator*(Value& V){
        std::vector<Value*> children = {this, &V};
        Value out(data * V.data, children, "*");
        out._backward = [&](){
            this->grad += V.data * out.grad;
            V.grad += this->data * out.grad;
        };
        return out;
    }

    void backward(){
        this->grad = 1.0;
        std::vector<Value*> topo = build_topo(this);
        for (size_t i = 0; i < topo.size(); i++){
            if(topo[i]->_backward){
                topo[i]->_backward();
            }
        }
    }
};

class neuron
{
public:
std::vector<Value*> w;
    neuron(int nin){
        for(int i = 0; i < nin; i++){
            w.push_back(new Value((rand() % 2) - 1));
        }
        Value b((rand() % 2) - 1);
    }

    float call(std::vector<float> x){
        // w * x + b
        for (size_t i = 0; i < w.size(); i++){
            
        }
        return 0.0;
    }
};


int main() {

    using namespace std;
    
    Value a(2.0);
    Value b(3.0);

    cout << "a: " << a.out() << endl;
    cout << "b: " << b.out() << endl;

    Value x(6.0);
    Value y(10.0);

    cout << "x: " << x.out() << endl;
    cout << "y: " << y.out() << endl;

    Value c = a * b;
    cout << "c: " << c.out() << endl;

    Value d = x + y;
    cout << "d: " << d.out() << endl;

    Value e = d + d;

    cout << "e: " << e.out() << endl;

    e.backward();


    cout << "a grad: " << a.grad << endl;
    cout << "b grad: " << b.grad << endl;
    cout << "x grad: " << x.grad << endl;
    cout << "y grad: " << y.grad << endl;
    cout << "c grad: " << c.grad << endl;
    cout << "d grad: " << d.grad << endl;
    cout << "e grad: " << e.grad << endl;



    return 0;
};