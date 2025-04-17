import request from '@/utils/request'

export const getUserList = (query: UserQuery & PageQuery): Promise<PaginateResponse<User>> => {
    if (Array.isArray(query.dateRange)) {
        query.dateRange = query.dateRange.join(',')
    }
    return request.get<Data<PaginateResponse<User>>, PaginateResponse<User>, UserQuery & PageQuery>(
        '/user',
        {
            params: query
        })
}

export const updateStatus = (userId: number, status: number): Promise<void> => {
    return request.put<void, void, void>(`/user/${userId}/status/${status}`)
}

export const newUser = (user: newUser): Promise<IdResponse> => {
    return request.post<Data<IdResponse>, IdResponse, newUser>('/user', user)
}

export const deleteUser = (userId: number): Promise<void> => {
    return request.delete<void, void, void>(`/user/${userId}`)
}

export const batchDeleteUser = (ids: number[], password: string): Promise<MessageResponse> => {
    return request.delete<Data<MessageResponse>, MessageResponse, BatchDeleteUserParams>('/user', { data: { ids, password } })
}

export const updateUser = (user: UserUpdate): Promise<void> => {
    return request.put<void, void, UserUpdate>(`/user/${user.user_id}`, user)
}