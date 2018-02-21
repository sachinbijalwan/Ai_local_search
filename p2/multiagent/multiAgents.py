# multiAgents.py
# --------------
# Licensing Information:  You are free to use or extend these projects for
# educational purposes provided that (1) you do not distribute or publish
# solutions, (2) you retain this notice, and (3) you provide clear
# attribution to UC Berkeley, including a link to http://ai.berkeley.edu.
# 
# Attribution Information: The Pacman AI projects were developed at UC Berkeley.
# The core projects and autograders were primarily created by John DeNero
# (denero@cs.berkeley.edu) and Dan Klein (klein@cs.berkeley.edu).
# Student side autograding was added by Brad Miller, Nick Hay, and
# Pieter Abbeel (pabbeel@cs.berkeley.edu).


from util import manhattanDistance
from game import Directions
import random, util

from game import Agent

class ReflexAgent(Agent):
    """
      A reflex agent chooses an action at each choice point by examining
      its alternatives via a state evaluation function.

      The code below is provided as a guide.  You are welcome to change
      it in any way you see fit, so long as you don't touch our method
      headers.
    """


    def getAction(self, gameState):
        """
        You do not need to change this method, but you're welcome to.

        getAction chooses among the best options according to the evaluation function.

        Just like in the previous project, getAction takes a GameState and returns
        some Directions.X for some X in the set {North, South, West, East, Stop}
        """
        # Collect legal moves and successor states
        legalMoves = gameState.getLegalActions()

        # Choose one of the best actions
        scores = [self.evaluationFunction(gameState, action) for action in legalMoves]
        bestScore = max(scores)
        bestIndices = [index for index in range(len(scores)) if scores[index] == bestScore]
        chosenIndex = random.choice(bestIndices) # Pick randomly among the best

        "Add more of your code here if you want to"

        return legalMoves[chosenIndex]

    def evaluationFunction(self, currentGameState, action):
        """
        Design a better evaluation function here.

        The evaluation function takes in the current and proposed successor
        GameStates (pacman.py) and returns a number, where higher numbers are better.

        The code below extracts some useful information from the state, like the
        remaining food (newFood) and Pacman position after moving (newPos).
        newScaredTimes holds the number of moves that each ghost will remain
        scared because of Pacman having eaten a power pellet.

        Print out these variables to see what you're getting, then combine them
        to create a masterful evaluation function.
        """
        # Useful information you can extract from a GameState (pacman.py)
        successorGameState = currentGameState.generatePacmanSuccessor(action)
        newPos = successorGameState.getPacmanPosition()
        newFood = successorGameState.getFood()
        newGhostStates = successorGameState.getGhostStates()
        newScaredTimes = [ghostState.scaredTimer for ghostState in newGhostStates]

        #print newScaredTimes

        "*** YOUR CODE HERE ***"
        foodList=newFood.asList()
        
        #Sum of manhattan distance of all food
        sum=0
        
        #Return  high negative if ghost is very close.
        for ghoststate in newGhostStates:
            if manhattandist(ghoststate.getPosition(),newPos)<=1:
                return -999999999
    
        #To prevent pacman from making no move at all
        if currentGameState.getPacmanPosition() == newPos:
            return -999999999
        
        #Sum of manhattan distance of all food
        for i in foodList:
            sum+=manhattandist(i,newPos)
        
        #If all foods are taken we return very high utility.
        if len(foodList)!=0:
            final=3400/sum + 24000/len(foodList)
        else:
            return 999999999

        return final

def manhattandist(xy1, xy2):
    return abs(xy1[0] - xy2[0]) + abs(xy1[1] - xy2[1])


def scoreEvaluationFunction(currentGameState):
    """
      This default evaluation function just returns the score of the state.
      The score is the same one displayed in the Pacman GUI.

      This evaluation function is meant for use with adversarial search agents
      (not reflex agents).
    """
    return currentGameState.getScore()

class MultiAgentSearchAgent(Agent):
    """
      This class provides some common elements to all of your
      multi-agent searchers.  Any methods defined here will be available
      to the MinimaxPacmanAgent, AlphaBetaPacmanAgent & ExpectimaxPacmanAgent.

      You *do not* need to make any changes here, but you can if you want to
      add functionality to all your adversarial search agents.  Please do not
      remove anything, however.

      Note: this is an abstract class: one that should not be instantiated.  It's
      only partially specified, and designed to be extended.  Agent (game.py)
      is another abstract class.
    """

    def __init__(self, evalFn = 'scoreEvaluationFunction', depth = '2'):
        self.index = 0 # Pacman is always agent index 0
        self.evaluationFunction = util.lookup(evalFn, globals())
        self.depth = int(depth)

class MinimaxAgent(MultiAgentSearchAgent):
    """
      Your minimax agent (question 2)
    """

    def getAction(self, gameState):
        """
          Returns the minimax action from the current gameState using self.depth
          and self.evaluationFunction.

          Here are some method calls that might be useful when implementing minimax.

          gameState.getLegalActions(agentIndex):
            Returns a list of legal actions for an agent
            agentIndex=0 means Pacman, ghosts are >= 1

          gameState.generateSuccessor(agentIndex, action):
            Returns the successor game state after an agent takes an action

          gameState.getNumAgents():
            Returns the total number of agents in the game
        """
        "*** YOUR CODE HERE ***"

        no_of_agents=gameState.getNumAgents()
        #Number of ghosts
        no_of_ghost = no_of_agents-1

        ans = self.minmax(gameState,self.depth,no_of_ghost)

        return ans
    
    def minmax(self,state,depth,ghost):

        legalMoves=state.getLegalActions(0)
        arr = []

        for action in legalMoves:
          arr.append(self.mini(state.generateSuccessor(0, action),depth,ghost,1))

        m=max(arr)

        bestIndices = []

        for index in range(len(arr)): 
          if arr[index] == m:
            bestIndices.append(index)

        chosenIndex = random.choice(bestIndices) # Pick randomly among the best
        return legalMoves[chosenIndex]


    def mini(self,state,depth,ghost,b):

        #Go to max agent if all min agents have been evaluated once.
        if(ghost==1):
            legalMoves=state.getLegalActions(b)
            arr = []

            for action in legalMoves:
              arr.append(self.maxi(state.generateSuccessor(b, action),depth-1,b))
           
            #If no successors return evaluation value
            if(arr==[]):
                return self.evaluationFunction(state)
            #Return min of all utility values of its successors
            return min(arr)

        else:
            legalMoves=state.getLegalActions(b)
            arr = []

            for action in legalMoves:
              arr.append(self.mini(state.generateSuccessor(b, action),depth,ghost-1,b+1)) 

            #If no successors return evaluation value
            if(arr==[]):
                return self.evaluationFunction(state)
            #Return min of all utility values of its successors
            return min(arr)


    def maxi(self,state,depth,ghost):
        #If given depth has been reached then return evaluation value
        if(depth==0):
            return self.evaluationFunction(state)

        legalMoves=state.getLegalActions(0)
        arr=[]

        for action in legalMoves:
          arr.append(self.mini(state.generateSuccessor(0, action),depth,ghost,1))

        #If no successors return evaluation value
        if(arr==[]):
            return self.evaluationFunction(state)
        #Return max of all utility values of its successors
        return max(arr)
    

class AlphaBetaAgent(MultiAgentSearchAgent):



    def getAction(self, gameState):
    
      no_of_agents=gameState.getNumAgents()
          #Number of ghosts
      no_of_ghost = no_of_agents-1

      ans = self.alphabeta(gameState,self.depth,no_of_ghost, -99999999,99999999)
      return ans

    
    
    def alphabeta(self,state,d,a,alpha,beta):
        legalMoves=state.getLegalActions(0)
        v=-9999999999
        for action in legalMoves:
            x=self.mini(state.generateSuccessor(0, action),d,a,1,alpha,beta)
            
            if x>=v:
                v=x
                ans=action
            if v<beta:
                alpha=max(alpha,v)
        return ans



    def mini(self,state,depth,ghost,b,alpha,beta):

        #Go to max agent if all min agents have been evaluated once.
      if(ghost==1):
        legalMoves=state.getLegalActions(b)

        if(legalMoves==[]):
            return self.evaluationFunction(state)

        v=999999999
            
        for action in legalMoves:
          v=min(v,self.maxi(state.generateSuccessor(b, action),depth-1,b,alpha,beta))

          if v>=alpha:
            beta=min(beta,v)
          else: return v

        return v

      else:
            legalMoves=state.getLegalActions(b)
            if(legalMoves==[]):
              return self.evaluationFunction(state)

            #Return min value among successors while it is in the expected range (alpha,beta)
            v=999999999
            for action in legalMoves:
                v=min(v,self.mini(state.generateSuccessor(b, action),depth,ghost-1,b+1,alpha,beta))
                if v>=alpha:
                    beta=min(beta,v)
                else: return v
            return v


    def maxi(self,state,depth,ghost,alpha , beta):
        #If given depth has been reached then return evaluation value
        if(depth==0):
            return self.evaluationFunction(state)

        legalMoves=state.getLegalActions(0)

        #If no successors return evaluation value
        if(legalMoves==[]):
            return self.evaluationFunction(state)

        v = -999999999
      

        for action in legalMoves:
          v=max(v,self.mini(state.generateSuccessor(0, action),depth,ghost,1,alpha,beta))
          if v<=beta:
              alpha=max(alpha,v)
          else: return v
        return v
        
        
    

   

class ExpectimaxAgent(MultiAgentSearchAgent):
    """
      Your expectimax agent (question 4)
    """

    def getAction(self, gameState):
        """
          Returns the expectimax action using self.depth and self.evaluationFunction

          All ghosts should be modeled as choosing uniformly at random from their
          legal moves.
        """
        "*** YOUR CODE HERE ***"
        no_of_agents=gameState.getNumAgents()
        #Number of ghosts
        no_of_ghost = no_of_agents-1
        ans = self.minmax(gameState,self.depth,no_of_ghost)
        return ans
    
    def minmax(self,state,depth,ghost):

        legalMoves=state.getLegalActions(0)

        arr=[self.mini(state.generateSuccessor(0, action),depth,ghost,1) for action in legalMoves]
        m=max(arr)

        bestIndices = []
        for index in range(len(arr)): 
          if arr[index] == m:
            bestIndices.append(index)

        chosenIndex = random.choice(bestIndices) # Pick randomly among the best
        return legalMoves[chosenIndex]


    def mini(self,state,depth,ghost,b):
        #Go to max agent if all min agents have been evaluated once.
        if(ghost==1):
            legalMoves=state.getLegalActions(b)
            arr=[self.maxi(state.generateSuccessor(b, action),depth-1,b) for action in legalMoves]
            #If no successors return evaluation value
            if(arr==[]):
                return self.evaluationFunction(state)
            sum=0.0
            #Return average value of successors as all equally likely(as ghosts play random moves).
            for x in arr:
                sum+=x
            return float(float(sum)/float(len(arr)))
        else:
            legalMoves=state.getLegalActions(b)
            arr=[self.mini(state.generateSuccessor(b, action),depth,ghost-1,b+1) for action in legalMoves]
            #If no successors return evaluation value
            if(arr==[]):
                return self.evaluationFunction(state)
            sum=0.0
            #Return average value of successors as all equally likely(as ghosts play random moves).
            for x in arr:
                sum+=x
            return float(float(sum)/float(len(arr)))


    def maxi(self,state,depth,ghost):
        #If given depth has been reached then return evaluation value
        if(depth==0):
            return self.evaluationFunction(state)

        legalMoves=state.getLegalActions(0)

        arr=[self.mini(state.generateSuccessor(0, action),depth,ghost,1) for action in legalMoves]
        #If no successors return evaluation value
        if(arr==[]):
            return self.evaluationFunction(state)
        #Return max of all utility values of its successors
        return max(arr)

def betterEvaluationFunction(currentGameState):
    """
      Your extreme ghost-hunting, pellet-nabbing, food-gobbling, unstoppable
      evaluation function (question 5).

      DESCRIPTION: <write something here so we know what you did>
    """
    "*** YOUR CODE HERE ***"
    "*** YOUR CODE HERE ***"
    
    newPos = currentGameState.getPacmanPosition()
    newFood = currentGameState.getFood()
    newGhostStates = currentGameState.getGhostStates()
    newScaredTimes = [ghostState.scaredTimer for ghostState in newGhostStates]
    
    "*** YOUR CODE HERE ***"
    

    #List of coordinates of all food particles
    food=newFood.asList()
    #sum of manhattan distance of all food particles
    sum=0
    
    
    #Gives a very negative utility if ghost is too close to pacman
    for g in newGhostStates:
        if manhattandist(g.getPosition(),newPos)<=2:
            return -9999999
        

    #Stores manhattan distance to ghost if the ghost is in scared state
    x=0
    if newScaredTimes>0:
      x=manhattandist(g.getPosition(),newPos)

    #sum of manhattan distance of all food particles
    for i in food:
      sum=sum+manhattandist(i,newPos)

    #Returns a very high utility if all food have been consumed
    if len(food)==0:
      return 999999999

    
    
    final=3400/sum + 26000/len(food) + 3500/x

    return final
better = betterEvaluationFunction

