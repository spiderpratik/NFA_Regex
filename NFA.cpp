#include <iostream>
#include <stdio.h>
#include <vector>
#include <string.h>
#include <malloc.h>

using namespace std;

bool belongs_to(int x, vector<int> v, int size)     //to check if element 'x' is present
{                                                   //in vector<int> 'v' (of size 'size')
    for (int i = 0; i < size; i++)
        if (v[i] == x)
            return true;
    return false;
}

bool equal(vector<int> v1, vector<int> v2)          //to check if 2 vectors contain the same elements
{
    if (v1.size() != v2.size())
        return false;
    else
    {
        if (v1.size() == 0)
            return true;
        vector<int> tmp;
        for (int i = 1; i <= v1[0]; i++)
            tmp.push_back(v1[i]);
        for (int i = 1; i <= v1[0]; i++)
            if (!belongs_to(v1[i], tmp, v2[0]))
                return false;
        return true;
    }
}

class Map
{
    private:
    vector<int> CS;     //Current State
    vector<char> in;    //Input character
    vector<int> NS;     //Next State
    int size;           //No. of transition mappings
    
    public:
    Map()                           //Default Constructor
    {
        size = 0;
    }
    
    int get_size()                      //returns size
    {
        return size;
    }
    
    bool add(int cs, char inp, int ns)      //add an entry to Map
    {
        if (!('a' <= inp && inp <= 'z') && inp != '#')
            return false;
        for (int i = 0; i < size; i++)
            if (CS[i] == cs && in[i] == inp && NS[i] == ns)
                return false;
        CS.push_back(cs);
        in.push_back(inp);
        NS.push_back(ns);
        size++;
        return true;
    }
    
    int transition(int cs, char inp)        //get transition from 'cs' on 'inp'
    {
        if (!('a' <= inp && inp <= 'z') && inp != '#')
            return -1;
        for (int i = 0; i < size; i++)
        {
            if (CS[i] == cs && in[i] == inp)
                return NS[i];
        }
        return -1;
    }
    
    int get_CS(int i)                       //get CS of i-th entry
    {
        if (i<size)
            return CS[i];
        else return -1;
    }
    
    int get_in(int i)                       //get in of i-th entry
    {
        if (i<size)
            return in[i];
        else return '\0';
    }
    
    int get_NS(int i)                       //get NS of i-th entry
    {
        if (i<size)
            return NS[i];
        else return -1;
    }
    
    int search(int cs, char inp, int ns)    //get index of entry (cs,in,ns)
    {
        if (!('a' <= inp && inp <= 'z') && inp != '#')
            return -1;
        for (int i = 0; i < size; i++)
            if (CS[i] == cs && in[i] == inp && NS[i] == ns)
                return i;
        return -1;
    }
    
    vector<int> search(int cs, char inp)    //get indices of all transitions from 'cs' on 'in'
    {
        vector<int> v;
        if (!('a' <= inp && inp <= 'z') && inp != '#')
        {
            cerr << "Invalid Input 8: " << inp << endl;
            v.insert(v.begin(), -1);
            return v;
        }
        int x = 1;
        for (int i = 0; i < size; i++)
        {
            if (CS[i] == cs && in[i] == inp)
            {
                v.push_back(i);
                x++;
            }
        }
        v.insert(v.begin(), x-1);
        return v;
    }
    
    vector<int> delta_prime(int cs, char inp)   //delta-prime function: get set of next states (ns)
    {                                           //from current state (cs) on input (in)
        vector<int> v;
        if (!('a' <= inp && inp <= 'z') && inp != '#')
        {
            cerr << "Invalid Input 7: " << inp << endl;
            v.insert(v.begin(), -1);
            return v;
        }
        int x = 0;
        for (int i = 0; i < size; i++)
        {
            if (CS[i] == cs && in[i] == inp)
            {
                if (!belongs_to(NS[i], v, x))
                {
                    v.push_back(NS[i]);
                    x++;
                }
            }
        }
        if (inp == '#')
        {
            v.push_back(cs);
            x++;
        }
        for (int i = 1; i < x; i++)
        {
            vector<int> t = search(v[i], '#');
            for (int j = 1; j <= t[0]; j++)
            {
                int extra = get_NS(t[j]);
                if (!belongs_to(extra, v, x))
                {
                    v.push_back(extra);
                    x++;
                }
            }
        }
        v.insert(v.begin(), x);
        return v;
    }
    
    bool remove(int i)                      //delete i-th entry
    {
        if (i >= size)
            return false;
        for (; i<size-1; i++)
        {
            CS.erase(CS.begin()+i);
            in.erase(in.begin()+i);
            NS.erase(NS.begin()+i);
        }
        size--;
        return true;
    }
    
    void print(int i)
    {
        if (i<size)
            cout << i << "(" << CS[i] << "," << in[i] << "," << NS[i] << ")" << endl;
        else cout << i << "(.)" << endl;
    }
    
    void pre_epsilon()                      //adds an extra state before the start state as the new
    {                                       //start state with an epsilon transition to the old state
        for (int i = 0; i < size; i++)
        {
            CS[i]++;
            NS[i]++;
        }
        add(0, '#', 1);
    }
};

class Automaton
{
    private:
    Map map;//Transitions mapping function
    //start state is always 0
    int F;  //final state (always the last state-indicates size)
        
    public:
    void print()
    {
        cout << "0->" << F << endl;
        for (int i = 0; i < map.get_size(); i++)
            map.print(i);
    }

    int get_final()
    {
        return F;
    }
    
    int get_CS(int i)
    {
        return map.get_CS(i);
    }
    
    char get_in(int i)
    {
        return map.get_in(i);
    }
    
    int get_NS(int i)
    {
        return map.get_NS(i);
    }
    
    Automaton(char c)                   //Creation of Automaton from character
    {
        if (c=='#')
            F=0;
        else
        {
            F=1;
            map.add(0,c,1);
        }
    }
    
    void concat(Automaton at)               //concatenation of current automaton with 'at'
    {
        for (int i = 0; i < at.map.get_size(); i++)
            map.add(at.get_CS(i)+F, at.get_in(i), at.get_NS(i)+F);
        F = F + at.F;
    }
    
    void kleene()                           //To construct kleene closure of current NFA
    {
        map.pre_epsilon();
        F++;
        map.add(0, '#', F+1);
        map.add(F, '#', 1);
        map.add(F, '#', F+1);
        F++;
    }
    
    void merge(Automaton at)                //union of current automaton with 'at' (+)
    {
        for (int i = 0; i < at.map.get_size(); i++)
            map.add(at.get_CS(i)+F+1, at.get_in(i), at.get_NS(i)+F+1);
        map.add(0, '#', F+1);
        map.add(F, '#', F+at.F+1);
        F = F + at.F + 1;
    }
    
    vector<int> delta(vector<int> cs, char c)       //delta fuction (extension of delta_prime)
    {
        vector<int> ns;
        int x = 0;
        for (int i = 1; i <= cs[0]; i++)
        {
            vector<int> tmp = map.delta_prime(cs[i], c);
            for (int j = 1; j <= tmp[0]; j++)
            {
                if (!belongs_to(tmp[j], ns, x))
                {
                    ns.push_back(tmp[j]);
                    x++;
                }
            }
        }
        ns.insert(ns.begin(), x);
        return ns;
    }
    
    vector<int> delta_hat(char* s)  //delta hat fuction (extension of delta) (assuming always from start state)
    {
        vector<int> states;
        states.push_back(1);
        states.push_back(0);
        while (true)
        {
            vector<int> tmp = delta(states, '#');
            if (equal(tmp, states))
                break;
            else states = tmp;
        }
        while (s[0]!='\0')
        {
            states = delta(states, s[0]);
            while (true)
            {
                vector<int> tmp = delta(states, '#');
                if (equal(tmp, states))
                    break;
                else states = tmp;
            }
            s++;
        }
        return states;
    }
};

bool isletter(char c)               //check if c is a letter(lowercase only) of '#'
{
    return ('a' <= c && c <= 'z') || c == '#';
}

bool validate(char* str)            //Regex Validation
{
    char* s = str;
    bool flag_bracket = false;
    while (strlen(s) > 1)
    {
        if (s[0] == '(')
        {
            if (flag_bracket)
            {
                cerr << "double bracket" << endl;
                return false;
            }
            else flag_bracket = true;
            if (!isletter(s[1]))
            {
                cerr << "*1*" << s[0] << s[1] << endl;
                return false;
            }
            s++;
            continue;
        }
        else if (s[0] == ')')
        {
            if (!flag_bracket || !(isletter(s[-1]) || s[-1] == '*'))
            {
                cerr << "double bracket" << endl;
                return false;
            }
            else flag_bracket = false;
            s++;
            continue;
        }
        else if (s[0] == '+')
        {
            if (s[1] == '(' || isletter(s[1]))
            {
                s++;
                continue;
            }
            else
            {
                cerr << "*2*" << s[0] << s[1] << endl;
                return false;
            }
        }
        else if (isletter(s[0]))
        {
            if (isletter(s[1]) || s[1] == '+' || s[1] == '(' || s[1] == ')' || s[1] == '*')
            {
                s++;
                continue;
            }
            else
            {
                cerr << "*3*" << s[0] << s[1] << endl;
                return false;
            }
        }
        else if (s[0] == '*')
        {
            if (isletter(s[1]) || s[1] == '+' || s[1] == '(' || s[1] == ')')
            {
                s++;
                continue;
            }
            else
            {
                cerr << "*4*" << s[0] << s[1] << endl;
                return false;
            }
        }
        else
        {
            cerr << s[0] << endl;
            return false;
        }
    }
    if (s[0] == ')' && flag_bracket)
        flag_bracket = false;
    else if (!isletter(s[0]) && s[0] != '*')
    {
        cerr << "*5*" << s[0] << endl;
        return false;
    }
    if (flag_bracket)
    {
        cerr << "bracket unmatched" << endl;
        return false;
    }
    return true;
}

char* simplify(char* str)
{
    char* s=str;
    char* ans = (char*) malloc(sizeof(char)*strlen(str)*2);
    char* a = ans;
    for (int i = 0; i < strlen(str)*2; i++)
        ans[i] = '\0';
    int offset = 0;
    while (s[0] != '\0')
    {
        if (s[0] == '#')
        {
            if (isletter(s[1]))
            {
                a[0] = s[1];
                a++;
                s+=2;
            }
            else if (s[1] == '*')
            {
                s++;
            }
            else
            {
                a[0] = s[0];
                a++;
                s++;
            }
        }
        else if (isletter(s[0]) || s[0] == '*' || s[0] == ')')
        {
            if (s[1] == '#')
                for (int i = 1; i < strlen(s); i++)
                    s[i] = s[i+1];
            else if (isletter(s[1]) || s[1] == '(')
            {
                a[0] = s[0];
                a[1] = '.';
                a+=2;
                s++;
            }
            else
            {
                a[0] = s[0];
                a++;
                s++;
            }
        }
        else
        {
            a[0] = s[0];
            a++;
            s++;
        }
    }
    return ans;
}

Automaton create_nfa(char* regex)
{
    vector<Automaton> at;
    vector<char> op;
    int atsize = 0, opsize = 0;
    for (int i = 0; regex[i] != '\0'; i++)
    {
        if ((regex[i] >= 'a' && regex[i] <= 'z') || regex[i] == '#')
        {
            Automaton tmp(regex[i]);
            at.push_back(tmp);
            atsize++;
        }
        else if (regex[i] == '+' || regex[i] == '.')
        {
            op.push_back(regex[i]);
            opsize++;
        }
        else if (regex[i] == '*')
        {
            at[atsize-1].kleene();
        }
        else if (regex[i] == '(')
        {
            int mark = i+1;
            i++;
            while (regex[i] != ')') i++;
            regex[i] = '\0';
            Automaton tmp = create_nfa(regex+mark);
            at.push_back(tmp);
            atsize++;
            regex[i] = ')';
        }
        else 
        {
            cerr << "Invalid Input4: " << regex[i] << endl;
            Automaton tmp('#');
            return tmp;
        }
    }
    if (atsize != opsize+1)
    {
        cerr << "Invalid Input5: " << (atsize - opsize) << endl;
        Automaton tmp('#');
        return tmp;
    }
    for (int i = 0; i < opsize;)
    {
        if (op[i] == '+')
            i++;
        else if (op[i] == '.')
        {
            at[i].concat(at[i+1]);
            at.erase(at.begin()+i+1);
            op.erase(op.begin()+i);
            atsize--;
            opsize--;
        }
        else 
        {
            cerr << "Invalid Input2: " << regex[i] << endl;
            Automaton tmp('#');
            return tmp;
        }
    }
    for (int i = 0; i < opsize; i++)
    {
        if (op[i] != '+')
        {
            cerr << "Invalid Input3: " << regex[i] << endl;
            Automaton tmp('#');
            return tmp;
        }
        at[0].merge(at[i+1]);
    }
    return at[0];
}

int main()                      //main function
{
    char regex[1000];
    scanf("%s", regex);
    if (!validate(regex))
    {
        cerr << "Regex Invalid" << endl;
        return 1;
    }
    char* str = simplify(regex);
    Automaton at = create_nfa(str);
    int n;
    cin >> n;
    while (n-- > 0)
    {
        string s;
        cin >> s;
        char str[s.length()+1];
        for (int i = 0; i < s.length(); i++)
            str[i] = s[i];
        str[s.length()] = '\0';
        vector<int> final = at.delta_hat(str);
        bool flag = true;
        for (int i = 1; i <= final[0]; i++)
        {
            if (final[i] == at.get_final())
            {
                cout << "Yes" << endl;
                flag = false;
                break;
            }
        }
        if (flag)
            cout << "No" << endl;
    }
    return 0;
}

