/*
PrivMX Endpoint.
Copyright © 2024 Simplito sp. z o.o.

This file is part of the PrivMX Platform (https://privmx.dev).
This software is Licensed under the PrivMX Free License.

See the License for the specific language governing permissions and
limitations under the License.
*/

#include <iostream>
#include <fstream>
#include <Poco/JSON/Object.h>
#include <Pson/Decoder.hpp>

using namespace std;
using Pson::Decoder;
using Poco::Dynamic::Var;

int main(int argc, char *argv[]){
    istream *input = &cin;

    if(argc > 1){
        input = new ifstream(argv[1]);
    }

    Decoder decoder;

    Var json = decoder.decode(*input);

    cout << json.toString() << endl;

    return 0;
}
