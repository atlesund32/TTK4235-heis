%vim: set filetype=tex
\ProvidesClass{itkoving}

\LoadClass[a4paper, 12pt, english]{article}

\RequirePackage[USenglish]{babel}
\RequirePackage{amsmath,amssymb, bm}
\RequirePackage[utf8]{inputenc}
\RequirePackage{graphicx}
\RequirePackage{multirow}
\RequirePackage{fullpage}
\RequirePackage{fancyhdr}
\RequirePackage{lastpage}
\RequirePackage{tcolorbox}
\RequirePackage{fmtcount}
\RequirePackage{refcount}

% Conditional for showing solution 
\newif\ifsolution
\solutionfalse
\DeclareOption{show-solution}{
    \solutiontrue 
}
\ProcessOptions

\newcommand{\exnum}[1]{\def \@exnum {#1}}
\newcommand{\extitle}[1]{\def \@extitle {#1}}
\newcommand{\exyear}[1]{\def \@exyear {#1}}
\newcommand{\exsem}[1]{\def \@exsem {#1}}
\newcommand{\exsub}[1]{\def \@exsub {#1}}

% Create Footer
\fancypagestyle{plain}{%
  \fancyhf{} % clear all header and footer fields
  \fancyfoot[L]{\@author}
  \fancyfoot[R]{Page\ \thepage\ of\ \pageref{LastPage}} % except the center
    \renewcommand{\headrulewidth}{0pt}
  \renewcommand{\footrulewidth}{0.4pt}
}
\pagestyle{plain}

\newcounter{exprob}
\newcounter{exsubprob}

\newcommand{\problem}[2]{\refstepcounter{exprob} \setcounter{exsubprob}{0} \subsubsection*{Problem \arabic{exprob} (#1 \%) #2}}

\newenvironment{subprob}
{\refstepcounter{exsubprob}
  \begin{enumerate}
    \item[\textbf{\alph{exsubprob}}]}
{\end{enumerate}}

\NewEnviron{solution}{
    \ifsolution
        \begin{tcolorbox}[title={Answer \arabic{exprob}\alph{exsubprob}}]
        \expandafter \BODY
        \end{tcolorbox}
    \else
    \fi
}


\renewcommand{\maketitle}{
  \begin{flushleft}
  \begin{tabular}{|lr|}
  \hline
    \multirow{3}{*}{\includegraphics{figures/ntnulogo_blue.pdf}} & \rule{0pt}{3ex} \@exsub \\& \@exsem\ \@exyear \\& \\
    \multicolumn{2}{|c|}{\rule{0.9737\textwidth}{0pt}} \\
    \footnotesize Norges teknisk-naturvitenskapelige universitet & \textbf{\ifsolution Solution \else Lab \fi \@exnum} \\
    \footnotesize Institutt for teknisk kybernetikk	& \@extitle	\\
    \hline 
    \end{tabular}
  \end{flushleft}
}