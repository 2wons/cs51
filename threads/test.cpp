#include <iostream>

using std::cin;
using std::cout;
using std::endl;
using std::string;


void impl()
{
    // input.txt to stdin
    freopen("input.txt","r",stdin);
}

void solve()
{
    int m,n,t;
    cin >> m >> n >> t;
    
    cout << m << endl;
    cout << n << endl;
    cout << t << endl;
}

int main()
{
    impl();

    solve();
    
}