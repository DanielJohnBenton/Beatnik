function ScoreLetter(letter)
{
	switch(letter)
	{
		case 'E':
		case 'A':
		case 'I':
		case 'O':
		case 'N':
		case 'R':
		case 'T':
		case 'L':
		case 'S':
		case 'U':
		{
			return 1;
		}
		
		case 'D':
		case 'G':
		{
			return 2;
		}
		
		case 'B':
		case 'C':
		case 'M':
		case 'P':
		{
			return 3;
		}
		
		case 'F':
		case 'H':
		case 'V':
		case 'W':
		case 'Y':
		{
			return 4;
		}
		
		case 'K':
		{
			return 5;
		}
		
		case 'J':
		case 'X':
		{
			return 8;
		}
		
		case 'Q':
		case 'Z':
		{
			return 10;
		}
		
		case '&':
		case '0':
		case '1':
		case '2':
		case '3':
		case '4':
		case '5':
		case '6':
		case '7':
		case '8':
		case '9':
		{
			return 0;
		}
		
		default:
		{
			return 0;
		}
	}
}

function ScoreWord(word)
{
	var score = 0;
	
	for(var i = 0, l = word.length; i < l; i++)
	{
		score += ScoreLetter(word.charAt(i));
	}
	
	return score;
}

function Operation(score)
{
	switch(score)
	{
		case 5:
			return "push(n)";
		
		case 6:
			return "pop()";
		
		case 7:
			return "push(pop() + pop())"
		
		case 8:
			return "push(input())";
		
		case 9:
			return "print(pop())";
		
		case 10:
			return "push(pop() - pop())";
		
		case 11:
			return "a = pop(); b = pop(); push(a); push(b)";
		
		case 12:
			return "a = pop(); push(a); push(a)";
		
		case 13:
			return "if(pop()==0) jump(+n+1)";
		
		case 14:
			return "if(pop()!=0) jump(+n+1)";
		
		case 15:
			return "if(pop()==0) jump(-n)";
		
		case 16:
			return "if(pop()!=0) jump(-n)";
		
		case 17:
			return "exit()";
		
		default:
			return "NOOP";
	}
}

function ScoreWords(words)
{
	for(var i = 0, n = words.length; i < n; i++)
	{
		words[i] = {
			word: words[i],
			score: ScoreWord(words[i])
		};
	}
	
	return words;
}

function Sanitise(text)
{
	text = text.toUpperCase();
	
	var sanitised = "";
	var space = true;
	
	for(var i = 0, l = text.length; i < l; i++)
	{
		var c = text.charAt(i);
		
		if(c == " " || c == "-" || c == "\t" || c == "\n" || c == "\r")
		{
			if(!space)
			{
				sanitised +=" ";
				space = true;
			}
		}
		else if("ABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789".indexOf(c) != -1)
		{
			sanitised += c;
			space = false;
		}
		else if(c == "&")
		{
			if(!space)
			{
				sanitised +=" ";
			}
			
			sanitised +="& ";
			
			space = true;
		}
		else if(c == ".")
		{
			if(!space && i >= 1 && text.charAt(i - 1) == ".")
			{
				sanitised +=" ";
				space = true;
			}
		}
	}
	
	return sanitised.trim();
}

function Explain(beatnik)
{
	var words = Sanitise(beatnik).split(" ")
	
	var div_explanation = document.getElementById("div_explanation");
	
	if(words.length == 0)
	{
		div_explanation.innerHTML = "";
	}
	
	words = ScoreWords(words);
	
	var html = "<table cellpadding='5' cellspacing='0' border='1'>"
		+"<tr><th>#</th><th>Word</th><th>Score</th><th>Operation</th></tr>";
	
	var warning = false;
	
	for(let i = 0, n = words.length; i < n; i++)
	{
		var word = words[i].word;
		var score = words[i].score;
		var operation = Operation(words[i].score);
		var number = i + 1;
		
		if(score == 5 || score >= 13 && score <= 16)
		{
			var x = i + 1;
			
			if(x >= n)
			{
				warning = true;
			}
			else
			{
				warning = false;
				
				if(score == 5)
				{
					operation = "push("+ words[x].score +")";
				}
				else
				{
					operation = "if(pop()";
					
					if(score == 13)
					{
						operation +="==0)<br />GOTO "+ (number + words[x].score + 1);
					}
					else if(score == 14)
					{
						operation +="!=0)<br />GOTO "+ (number + words[x].score + 1);
					}
					else if(score == 15)
					{
						operation +="==0)<br />GOTO "+ (number - (words[x].score));
					}
					else if(score == 16)
					{
						operation +="!=0)<br />GOTO "+ (number - (words[x].score));
					}
				}
				
				word +="<br />"+ words[x].word;
				score +="<br />"+ words[x].score;
				
				number +="<br />"+ (number + 1);
				
				i++;
			}
		}
		else
		{
			warning = false;
		}
		
		html +="<tr><td>"+ number +"</td><td>"+ word +"</td><td>"+ score +"</td><td>"+ operation +"</td></tr>";
	}
	
	html +="</table>";
	
	div_explanation.innerHTML = html;
	
	var div_warning = document.getElementById("div_warning");
	if(warning)
	{
		div_warning.innerHTML = "<br /><b>A PARAMETER IS REQUIRED!</b>"
	}
	else
	{
		div_warning.innerHTML = "";
	}
}

function ShowLetterScores()
{
	var div_letters = document.getElementById("div_letters");
	
	div_letters.innerHTML ="<span onclick='HideLetterScores()'>Hide letter scores &lt; &lt;</span><br />"
		+"<table cellpadding='5' cellspacing='0' border='1'>"
		+"<tr><td>AEIOU NRTLS</td><td>1</td></tr>"
		+"<tr><td>DG</td><td>2</td></tr>"
		+"<tr><td>BCMP</td><td>3</td></tr>"
		+"<tr><td>FHVWY</td><td>4</td></tr>"
		+"<tr><td>K</td><td>5</td></tr>"
		+"<tr><td>JX</td><td>8</td></tr>"
		+"<tr><td>QZ</td><td>10</td></tr>"
		+"</table><br />";
}

function HideLetterScores()
{
	var div_letters = document.getElementById("div_letters");
	
	div_letters.innerHTML = "<span onclick='ShowLetterScores()'>Show letter scores &gt; &gt;</span>";
}
















