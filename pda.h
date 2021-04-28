#ifndef PDA_HPP
#define PDA_HPP

#include <set>
#include <map>
#include <list>
#include <stack>
#include <string>
#include <ostream>
#include <QDomElement>
#include "tree.h"
#include <gvc.h>
#include <cgraph.h>

class PDA {

public:

    using StackSymbol = char;
    using State = int;

    struct Transition {
        State from;
        std::string symbols;
        std::vector<State> to;
        std::string pop;
        std::string push;
    };

    using Transitions = std::vector<Transition>;

    struct Configuration {
        int state;
        std::string remaining;
        std::list<StackSymbol> stack;
        bool operator< (const Configuration& other) const;
        bool operator== (const Configuration& other) const;
        bool starts_with(std::string symbols) const;
        bool are_top(std::string symbols) const;
        QString createOutputString();
    };

    PDA(std::set<State> states, State start_state, std::set<State> accept_states, Transitions transitions);
    PDA();

    State getStartState() const;

    bool isAcceptingState(State state) const;

    std::set<Configuration> yieldInOneStep(Configuration& c) const;

    bool isAccepted(std::string word) const;

    TreeNode<PDA::Configuration> getConfigurationTree(std::string word) const;

    bool canMove(const Configuration& c, const Transition& t) const;

    Configuration move(const Configuration& c, const Transition& t) const;


    std::set<State> getStates() const;

    std::set<State> getAcceptStates() const;

    Transitions getTransitions() const;

    void writeXML();

    void setStartState(const State &value);

    void setStates(const State &value);

    void setTransitions(const Transition &value);

    void setAcceptStates(const State &value);

    bool saveImageGV(std::string file_path);

    void toDot(QString filename);

    QString acceptStatesToDot();
    QString transitionsToDot();

private:
    std::set<State> states;
    State start_state;
    std::set<State> acceptStates;
    Transitions transitions;

    void expand(TreeNode<Configuration>& configuration_tree) const;
};

std::ostream& operator<< (std::ostream& s, const PDA::Configuration& c);

#endif
