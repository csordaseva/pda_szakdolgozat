#include "pda.h"

using State = int;
using StackSymbol = char;
using Transitions = std::vector<PDA::Transition>;

bool PDA::Configuration::operator< (const Configuration& other) const {
    return state < other.state || (state == other.state && remaining < other.remaining)
            || (state == other.state && remaining == other.remaining && stack < other.stack);
}

bool PDA::Configuration::operator== (const Configuration& other) const {
    return state == other.state && remaining == other.remaining && stack == other.stack;
}

PDA::PDA(std::set<State> states, State start_state, std::set<State> accept_states, Transitions transitions)
    : states(states), start_state(start_state), acceptStates(accept_states), transitions(transitions) {}

PDA::PDA(){}

State PDA::getStartState() const {
    return start_state;
}

bool PDA::isAcceptingState(State state) const {
    return acceptStates.count(state) == 1;
}

bool PDA::isAccepted(std::string word) const {
    std::set<PDA::Configuration> open_configurations;
    open_configurations.insert(PDA::Configuration{ getStartState(),word, {} });
    try
    {
        while (!open_configurations.empty())
        {
            Configuration selected = *open_configurations.begin();
            open_configurations.erase(open_configurations.begin());
            if (isAcceptingState(selected.state) && selected.remaining.empty() && selected.stack.empty())
            {
                throw true;
            }
            else
            {
                open_configurations.merge(yieldInOneStep(selected));
            }
        }
    }
    catch (bool)
    {
        return true;
    }
    return false;
}

std::set<PDA::Configuration> PDA::yieldInOneStep(Configuration& c) const {
    std::set<PDA::Configuration> new_configurations;
    for (auto& element : transitions)
    {
        if (canMove(c,element))
        {
            new_configurations.insert(move(c, element));
        }
    }
    return new_configurations;
}



void PDA::expand(TreeNode<Configuration>& configuration_tree) const {
    for (auto child_configuration : yieldInOneStep(configuration_tree.data))
    {
        TreeNode<Configuration> child{ child_configuration , {} };
        expand(child);
        configuration_tree.children.push_back(child);
    }
}

TreeNode<PDA::Configuration> PDA::getConfigurationTree(std::string word) const {
    TreeNode<Configuration> tree{ {getStartState(),word, {}}, {}};
    expand(tree);
    return tree;
}


std::ostream& operator<< (std::ostream& s, const PDA::Configuration& c){
    s << "( " << c.state << " , ";
    if (c.remaining.empty())
    {
        s << "E, ";
    }
    else
    {
        s << c.remaining << ",  ";
    }

    if (c.stack.empty())
    {
        s << "E";
    }
    else
    {
        std::list<StackSymbol>::const_iterator it;
        for (it = c.stack.begin(); it != c.stack.end(); ++it)
        {
            s << *it;
        }
    }
    s << ")";
    return s;
}

bool PDA::Configuration::starts_with(std::string symbols) const{
    if (remaining.size() < symbols.size())
    {
        return false;
    }
    else
    {
        for (unsigned int i = 0; i < symbols.size(); i++)
        {
            if (remaining[i] != symbols[i])
            {
                return false;
            }
        }
        return true;
    }

}

bool PDA::Configuration::are_top(std::string symbols) const{
    if (stack.size() < symbols.size())
    {
        return false;
    }
    else
    {
        std::list<StackSymbol>::const_iterator it;
        unsigned int i = 0;
        for (it = stack.begin(); i < symbols.size(); ++it)
        {
            if (*it != symbols[i])
            {
                return false;
            }
            i++;
        }
        return true;
    }
}

QString PDA::Configuration::createOutputString(){
    QString stackString{};
    for( auto it : stack)
    {
        stackString += it;
    }
    QString rem = QString::fromStdString( remaining );
    QString stack = stackString;
    if(QString::fromStdString( remaining ).isEmpty()){
        rem = "E";
    }
    if(stackString.isEmpty()){
        stack = "E";
    }
    return QString("(" + QString::number(state) + " , " + rem  + " , " + stack +")");
}

bool PDA::canMove(const Configuration& c, const Transition& t) const{
    return c.state == t.from && c.starts_with(t.symbols) && c.are_top(t.pop);
}

PDA::Configuration PDA::move(const Configuration& c, const Transition& t) const {
    if (canMove(c, t))
    {
        auto new_c = Configuration(c);
        for (unsigned int i = 0; i < t.to.size(); i++)
        {
            new_c.state = t.to[i];
            new_c.remaining = new_c.remaining.substr(t.symbols.size());

            for (unsigned int j = 0; j < t.pop.size(); j++)
            {
                new_c.stack.pop_front();
            }
            for (unsigned int j = t.push.size(); j > 0; j--)
            {
                new_c.stack.push_front(t.push[j-1]);
            }
        }
        return new_c;
    }
    return c;
}

std::set<State> PDA::getStates() const{
    return states;
}

std::set<State> PDA::getAcceptStates() const{
    return acceptStates;
}

Transitions PDA::getTransitions() const{
    return transitions;
}

void PDA::writeXML(){    
    QDomDocument document;
    QDomElement root = document.createElement("pda");
    document.appendChild(root);

    QDomElement statesNode = document.createElement("states");
    root.appendChild(statesNode);

    for(unsigned int i = 0; i < getStates().size(); i++)
    {
        QDomElement node = document.createElement("state");
        node.setAttribute("name", *next(getStates().begin(),i));
        statesNode.appendChild(node);
    }

    QDomElement startState = document.createElement("start_state");
    startState.setAttribute("name", QString::number(getStartState()));
    root.appendChild(startState);

    QDomElement acceptStates = document.createElement("acceptStates");
    root.appendChild(acceptStates);

    for(unsigned int i = 0; i < getAcceptStates().size(); i++)
    {
        QDomElement node = document.createElement("accept_state");
        node.setAttribute("name", *next(getAcceptStates().begin(),i));
        acceptStates.appendChild(node);
    }

    QDomElement transitions = document.createElement("transitions");
    root.appendChild(transitions);

    for(unsigned int i = 0; i < getTransitions().size(); i++)
    {
        QDomElement node = document.createElement("transition");
        for(unsigned int j = 0; j < getTransitions().at(i).to.size();j++)
        {
            node.setAttribute("from", getTransitions().at(i).from);
            node.setAttribute("symbol", QString::fromStdString(getTransitions().at(i).symbols));
            node.setAttribute("to", getTransitions().at(i).to.at(j));
            node.setAttribute("pop", QString::fromStdString(getTransitions().at(i).pop));
            node.setAttribute("push", QString::fromStdString(getTransitions().at(i).push));
            transitions.appendChild(node);
        }
    }

    QFile file("C:/Users/Éva/Desktop/pda.xml");
    if(!file.open(QIODevice::WriteOnly | QIODevice::Text))
    {
        qDebug() << "Failed to open file for writing.";
        return;
    }
    else
    {
        QTextStream stream(&file);
        stream << document.toString();
        file.close();
        qDebug() << "Finished.";
    }
}

void PDA::toSVG(QString dotfilename)
{
    QString command = "dot -Tsvg " + dotfilename + " -o " + dotfilename + ".svg";
    QProcess::execute(command);
    //qDebug() << "command executed";
}

void PDA::setStartState(const State &value){
    start_state = value;
}

void PDA::setStates(const State &value){
    states.insert(value);
}

void PDA::setTransitions(const Transition &value){
    transitions.push_back(value);
}

void PDA::setAcceptStates(const State &value){
    acceptStates.insert(value);
}

void PDA::toDot(QString filename){

    QFile file(filename);

    if(!file.open(QFile::WriteOnly | QFile::Text))
    {
        qDebug() << "Could not open file for writing.";
    }
    QTextStream out(&file);
    out << "digraph {\n" <<
           "\trankdir=\"TB\"\n" <<
           "\tnode [shape = circle, color=black]\n" <<
           "\tstart [label = \"\", shape=none, width=0]\n\t" <<
           acceptStatesToDot() <<
           "\t[shape=doublecircle]\n" <<
           "\tstart -> " << PDA::getStartState() <<
           transitionsToDot() <<
           "\n}";
    file.close();
}

QString PDA::acceptStatesToDot(){
    QString acceptStates;
    auto it = PDA::acceptStates.end();
    it--;
    for(auto as : PDA::getAcceptStates())
    {
        acceptStates += QString::number(as);
        if(as != *it)
        {
            acceptStates += ",";
        }
    }
    return acceptStates;
}

QString PDA::transitionsToDot(){
    QString transitionList;
    for(auto tr : PDA::getTransitions())
    {
        transitionList += "\n\t";
        if(QString::fromStdString(tr.symbols) == "" && tr.to.size() == 1)
        {
           transitionList += QString(QString::number(tr.from) + " -> " + QString::number(tr.to.front()) + "[label=\"e\"]");
        }
        else if(QString::fromStdString(tr.symbols) == "" && tr.to.size() > 1)
        {
           for(auto t : tr.to)
           {
               transitionList += QString(QString::number(tr.from) + " -> " + QString::number(t) + "[label=\"e\"]");
           }
        }
        else if(QString::fromStdString(tr.symbols) != "" && tr.to.size() == 1)
        {
            transitionList += QString(QString::number(tr.from) + " -> " + QString::number(tr.to.front()) + "[label=\"" + QString::fromStdString(tr.symbols) + "\"]");
        }
        else
        {
            for(auto t : tr.to)
            {
                transitionList += QString(QString::number(tr.from) + " -> " + QString::number(t) + "[label=\"e\"]");
            }
        }
    }
    return transitionList;
}
