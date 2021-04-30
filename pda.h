#ifndef PDA_HPP
#define PDA_HPP

#include <set>
#include <map>
#include <list>
#include <stack>
#include "string.h"
#include <ostream>
#include <QDomElement>
#include <QDomDocument>
#include <QFile>
#include <QDebug>
#include <QTreeWidgetItem>
#include "tree.h"
#include <gvc.h>
#include <cgraph.h>
#include <qprocess.h>

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

    PDA();

    PDA(std::set<State> states, State start_state, std::set<State> accept_states, Transitions transitions);

    State getStartState() const;

    std::set<State> getStates() const;

    std::set<State> getAcceptStates() const;

    Transitions getTransitions() const;

    void setStartState(const State &value);

    void setStates(const State &value);

    void setTransitions(const Transition &value);

    void setAcceptStates(const State &value);

    bool isAcceptingState(State state) const;

    std::set<Configuration> yieldInOneStep(Configuration& c) const;

    bool isAccepted(std::string word) const;

    TreeNode<PDA::Configuration> getConfigurationTree(std::string word) const;

    bool canMove(const Configuration& c, const Transition& t) const;

    Configuration move(const Configuration& c, const Transition& t) const;

    void writeXML();

    void toSVG(QString dotfilename);

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
