R"------(
EXAMPLES
	[selector_path] [attribute] for specified websites.

	BBC News Article
		https://web.archive.org/web/20201128041650/https://www.bbc.co.uk/news/world-middle-east-55111064
		Title:    "*@h1#main-heading" .
		Datetime: "*@time:datetime" datetime

	NYT Article
		Title:    "*@h1:itemprop=headline" . // Not @title .
		Subtitle: "*@h1:itemprop=headline>^^>+3" .
			Find the headline, then go up two nodes and get the 3rd child node
		Author:   "*@p:itemprop=author>@a>@span" .
		Datetime: "*@p:itemprop=author>^^^>@ul>*@time" datetime

	WSJ Article
		https://web.archive.org/web/20201125032555/https://www.wsj.com/articles/mayor-pete-and-the-purdue-charter-11580074911
		Title:    "*@h1.wsj-article-headline" .
		Subtitle: "*@h2.sub-head" .
		Author:   "*@div.author-container>@button" .
		Datetime: "*@div.byline>time" .
)------"
