import openai

if __name__ == "__main__":
    # 设置 API Key，申请地址：https://platform.openai.com/account/api-keys
    # openai.api_key = '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'
    openai.api_key = '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'
    # 设置组织，查看地址：https://platform.openai.com/account/org-settings
    openai.organization = "org-pXpXZEbQoBzGslRGiqX3g6un"
    # 请求模型，查看可用模型：https://platform.openai.com/docs/models/overview
    model_engine = "gpt-3.5-turbo"
    # 输入内容
    prompt = "The layout problem is: Given an $n \\times m$ grid and $k$ pieces, where each piece has a shape of $h_i \\times w_i$. Determine whether it is possible to place all the pieces on the grid such that each piece is completely included in the grid and there is no overlapping between pieces. Build a MiniZinc model for the layout problem. Please only output the model, without any prompt or explanation."
    # 调用接口
    # completions = openai.Completion.create(
    #     engine=model_engine,
    #     prompt=prompt,
    #     max_tokens=40000,
    #     n=1,
    #     stop=None,
    #     temperature=0.5,
    # )

    completions = openai.ChatCompletion.create(
        model="gpt-3.5-turbo",
        messages=[
            {"role": "user", "content": prompt}
        ]
    )

    # 输出结果
    message = completions.choices[0].message
    print(message)
