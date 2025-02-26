# DEC 12
This day was when we really started coding, after spending a bit of time setting up the whole project, i.e setting up the git repo, ssh, etc. and how we would represent our board and pieces, what structures we would use.
- Worked on pawn moves in the context of generating all possible moves for all pieces 
- Started trying to set up CMake (unsuccessfully)
- Did a lot of debugging, which ended with us realising that the way we were working was wrong (issues with polymorphisms)

# DEC 19
- Set up CMake (successfully this time though) which helped solve many bugs we had.

# DEC 20
- Went through some of our code and removed useless comments that were clogging up the workspace
- Corrected mistakes made in the functions generating possible moves for the knights, bishops and queen
- Added points attribute to each pieces according to the wiki
- We already had a map enemy_positions being created in intialize.cpp, I also added our_positions (consequentially, I had to update the signature of our functions)
- Created global_args file which would store our main function argc and argv so they could be reused. 
- Added new methods to pawn structures yet to be implemented

# OBJECTIVES FOR THE HOLDIAYS
- As we knew that we wouldn't be able to work together in person durign the holidays, we decided we had to proiritise a few things:
  - Finishing up our random move generator
  - Start thinking about how we would "choose" the next move
    - This involved a lot of research about chess engines, evaluation functions, beta pruning algorithms and trees and their use in this context. 

# DEC 27
- Updated CMake standard to avoid warnings when building
- Fixed castling error 
This was when we were done working on generating a random move and setting up most of the basic functionality of the project. We then started to work on the next step: evaluating which move was best. This started with a lot of discussion about what was the best way to do this, and then we started working on the evaluation function [see chess wiki](https://www.chessprogramming.org/Evaluation), which, using multiple different parameters, would evaluate the "value" of a given move/position. 
- Created function val_of_board to calculate material value of the board (using value of pieces). At this point I realised using a points attribute would not work, so I created a method for each piece that would be overriden to calculate the value of the piece, called get_points.

# DEC 28
- Implemented the mobility evaluation feature. This measures the number of legal moves a player's pieces can make. It reflects the freedom of movement and activity of pieces, with higher mobility indicating a more dynamic and flexible position. Very useful because it encourages active piece play and penalizes cramped or passive setups. Implemented/helped implement king shield and king safety functions to evaluate safety of king. King shield refers to the evaluation of the pawn structure directly in front of the king. A strong shield offers better protection, while weaknesses like missing pawns or open files near the king reduce safety. This helps the engine assess how secure the king is from immediate attack. King safety measures the overall safety of the king, considering factors like nearby enemy pieces, open lines, and threats. It combines the king’s shield and positional factors to estimate the likelihood of the king being exposed to checks, attacks, or mate threats. Ensuring king safety is crucial for survival, especially in the middlegame.

# DEC 30
- Implemented center control feature, which checks if we or the enemy have potential moves they can make in the center of the board. 
- Implemented space evaluation feature, which loops over all pieces, all of their possible moves and calculates how valuable they are depending on if they can control enemy territory, central positions or regular positions. We later realised that mobility and space evaluation and center control are sort of equivalent, so we removed the use of mobility and center control in the end.
- Added general comments "docstrings" for most functions in eval.cpp and eval.h for readability.

# JAN 8 
- Deleted global_args again (for what feels like the billionth time)
- Started working on optimising our evaluation function: change all function signatures to use references to avoid copying. The idea was that by doing this we would be able to avoid useless operations so that when searching through the tree to choose our best move, we could go deeper faster.

# JAN 9
- Optimised evaluation function by changing type identification from dyanmic casting to using a type() function that returns a string instead to avoid taking time on that
- Corrected some type errors and variable definitions in tree.cpp 

# JAN 13
- Changed a few function signatures 
- Created the mini_eval function. The idea here was to create a function that would only compute the value of the board for each move and return them sorted in descending order so that we would use them as an intermediate step to build our tree. This would generally make looking through the tree easier. The general risk we thought our engine might encore was that if our tree creation unfortunately placed the best moves as the rightmost child, then beta pruning would never actually occur! Hence rendering our beta pruning algorithm completely useless and also making it more computationally costly to choose a move. Therefore we decided to work on this function to optimise the position of the moves as the tree is being created, so that move determination would be faster and more accurate. 

# JAN 16
- Spent a while trying to figure out how I could use the mini_eval in building our tree. Corrected the beta pruning a bit and managed to end up with a way to create a functioning tree that used functional beta pruning to pick the best move based on the mini_eval function. (This would be scrapped later on)

# JAN 21
- Fixed the evaluation function to make it completely symmetrical. This involved going through all the evaluation paramters and testing if the logic was correct and a lot of std::cout to see the immediate result of each function. We created a function to switch the board around so that we could test the evaluation from the enemy's perspective and ensure its symmetry.

# JAN 22
- After Horace pushed his code changing much of the structure of the code, I adapted the evaluation function to make it work again, effectively changing all the code we wrote in it but keeping the same logic. The big difference is that in most of our functions (as you can still see in the ones that we commented out after realising that we wouldn't use them) we iterated over each piece and their possible moves and then calculated a score based on what we were looking to evaluate. However, after Horace refactored the code, the pieces no longer has a given set of moves. Instead, we had a vector of strings of moves. So the idea was to use this instead. This involved running through all the moves, taking the first two characters (a1 represents white left rook at the start of game) and using this to identify the piece at this position to get the type and THEN do the calculations. All in all this was not too difficult since the logic remained the same, but I still needed to rethink the structure of the code.
- We started thinking about changing all our vectors and maps to arrays (did not do it, too hard to refactor entire code)
- Disappointed to see half of the evaluation function didn't make it so we could still play in <= 10 seconds, but this was the crux of the problem: should we prioritise evaluation accuracy over depth recursion in the tree? In the end, we decided to go with depth, because we realised that even though our evaluation was very accurate for a given board, the issue would be that we could be overlooking the next "best" move which actually could be a terrible blunder! Therefore it made more sense to limit the number of evaluation parameters taken into account so that we could explore deeper. 