I first started working on the parsing of the text file we receive as an argument and the initial creation of the chessboard.
I thus first created an initialize chessboard function which returns a vector of pieces and maps that are needed for the implementation of perform move. We later realised that the usage of maps was very costly and that we should have
used arrays.
I then proceeded to implement the perform move function which receives a move, a board and maps and updates the latter according to the move which is being played.
I had some trouble with this function as at first I was making the conditions for certain specific moves (castling & en passant) too specfic. When I realized I could assume we would only receive legal moves I started to relax the conditions in order to get a more logically optimal code however this led to one more noticeable error which took me a lot of time to resolve.
In order to check for en passant and remove the pawn that is getting eaten I realized I only had to check if the piece that was moving was a pawn that was moving in a diagonal to a square with no opposing players.
However in my perform move function I would implement the 