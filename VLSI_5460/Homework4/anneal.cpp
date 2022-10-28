
int get_rand_operand_idx(string E, vector<node_t> nodes) {
    int rand_max = 1000;

    int nodes_size = nodes.size();
    // cout << "Node size: " << nodes_size << endl;
    int rand_idx = abs((rand() * rand_max) % nodes_size);
    // cout << "Rand Index: " << rand_idx << endl;
    node_t node = nodes[rand_idx];
    // cout << "Got node" << endl;
    char operand = node.id[0];
    // cout << "Random operand: " << operand << endl;

    int string_size = E.size();
    for (int i = 0; i < string_size; i++) {
        if (E[i] == operand) {
            return i;
        }
    }
    return -1;
}

bool is_operator(char c) {
    if (c == 'V' || c == 'H') {
        return true;
    }
    else {
        return false;
    }
}

/**
 * @brief Divdes one/two
 * @return float quotient
 */
float safe_div(float one, float two) {
    if (one == 0 || two == 0) {
        return 1;
    }
    else {
        return one/two;
    }
}

string make_move(string E, vector<node_t> nodes) {
    string NewE = E;
    int first;
    int second;
    int C;
    bool done;
    int pos;
    int rand_idx;
    int M = abs((rand() * 10) % 3);
    switch(M) {
        case 0: //Two operands
            // std::cout << "Operands: " << E << endl;
            first = get_rand_operand_idx(NewE, nodes);
            // cout << "First " << first << endl;
            second = -1;
            while ((second < 0) && (second != first)) {
                // cout << "Trying Second" << endl;
                second = get_rand_operand_idx(NewE, nodes);
            }
            // cout << "Second " << second << endl;
            swap(NewE[first], NewE[second]);
            // std::cout << "NewE:     " << NewE << endl;
            break;

        case 1: //Two operators
            // std::cout << "Operators: " << E << endl;
            // C = abs((rand() * 100) % (int)(NewE.size() / 2));
            // for(int i = 0; i < NewE.size() && C > 0; i++) {
            rand_idx = abs((rand() * 100) % (int)(NewE.size() / 2));
            for(int i = rand_idx; i < NewE.size(); i++) {
                if (NewE[i] == 'V') {
                    NewE[i] = 'H';
                    break;
                }
                else if (NewE[i] == 'H') {
                    NewE[i] = 'V';
                    break;
                }
            }
            // std::cout << "NewE:      " << NewE << endl;
            break;

        case 2: //Operands and operators
            // std::cout << "Mixed: " << E << endl;
            done = false;
            pos = 0;
            while (!done) {
                //rand starting pos
                pos = abs((rand() * 100) % (int)(E.size() / 2));
                // cout << "Pos: " << pos << endl;
                if (pos < 2) {
                    pos = 2;    //Can't have the first two characters not be operands
                }
                int i;
                for (i = pos; i < NewE.size() - 1; i++) {
                    if (is_operator(NewE[i]) && !is_operator(NewE[i + 1]) ||
                        !is_operator(NewE[i]) && is_operator(NewE[i + 1])) {
                            done = true;
                            break;
                        }
                }
                if (done) {
                    swap(NewE[i], NewE[i + 1]);
                }
            }
            // std::cout << "NewE:  " << NewE << endl;
            break;

        default:
            std::cout << "M out of bounds: " << M << endl;
            return "";
    }
    return NewE;
}

string anneal(const string E0, vector<node_t> nodes) {
    std::cout << "Anneal: " << E0 << endl;
    string E = E0;
    string Best = E0;
    string NewE = E0;
    float T = t0;
    if (T < 0) {
        T = 1000000;   //TODO This should be calculated into a large number
    }
    int uphill = 0;
    int MT = 0;
    int Reject = 0;
    bool out_of_time = false;
    int delta_cost = 0;

    while ((safe_div(Reject, MT) > 0.95) && (T > epsilon) && !out_of_time) {
        std::cout << "T: " << T << endl;
        MT = uphill = Reject = 0;
        // E = Best;
        NewE = E;
        while ((uphill <= nmoves) && (MT <= 2 * nmoves)) {
            NewE = make_move(NewE, nodes);
            MT++;
            // cout << "Evaluating" << endl;
            float new_cost = npe_cost(NewE, nodes);
            // cout << "New cost: " << new_cost << endl;
            float old_cost = npe_cost(E, nodes);
            // cout << "Old cost: " << old_cost << endl;
            delta_cost = npe_cost(NewE, nodes) - npe_cost(E, nodes);
            // cout << "delta_cost " << delta_cost << endl;
            //TODO: Track chip ratio
            if ((delta_cost < 0) || (rand() < exp(-delta_cost / T))) {
                if (delta_cost > 0) { uphill++; }
                //Accept NewE
                cout << "Accept NewE: " << NewE << endl;
                E = NewE;
                if (npe_cost(E, nodes) < npe_cost(Best, nodes)) {
                    //Save if is best so far
                    cout << "Best: " << E << endl;
                    Best = E;
                }
            }
            else {
                // cout << "Rejected: " << NewE << endl;
                Reject++;
            }
        }
        T *= ratio;
    }
    return Best;
}
